#include "SessionCallbacks.h"
#include "../Application.h"
#include "../objects/spotify/PlaylistContainer.h"
#include "../objects/spotify/Player.h"
#include "../utils/V8Utils.h"

extern "C" {
  #include "../audio/audio.h"
}

#include <node_buffer.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

extern Application* application;
static void handleNotify(uv_async_t* handle, int status);
static void handleMusicInNodeThread(uv_async_t* handle, int status);

static sp_playlistcontainer_callbacks rootPlaylistContainerCallbacks;
static std::unique_ptr<uv_timer_t> timer;
static std::unique_ptr<uv_async_t> notifyHandle;
static std::unique_ptr<uv_async_t> musicNotifyHandle;
v8::Handle<v8::Function> SessionCallbacks::loginCallback;
v8::Handle<v8::Function> SessionCallbacks::logoutCallback;
v8::Handle<v8::Function> SessionCallbacks::metadataUpdatedCallback;
v8::Handle<v8::Function> SessionCallbacks::endOfTrackCallback;
v8::Handle<v8::Function> SessionCallbacks::musicDeliveryCallback;

namespace spotify {
//TODO
int framesReceived = 0;
int currentSecond = 0;
}

struct music {
  size_t size;
  void* data;
};

void SessionCallbacks::init() {
  timer = std::unique_ptr<uv_timer_t>(new uv_timer_t());
  notifyHandle = std::unique_ptr<uv_async_t>(new uv_async_t());
  musicNotifyHandle = std::unique_ptr<uv_async_t>(new uv_async_t());
  uv_async_init(uv_default_loop(), notifyHandle.get(), handleNotify);
  uv_async_init(uv_default_loop(), musicNotifyHandle.get(), handleMusicInNodeThread);
  uv_timer_init(uv_default_loop(), timer.get());
}

/**
 * If the timer for sp_session_process_events has run out this method will be called.
 **/
static void handleTimeout(uv_timer_t* timer, int status) {
  handleNotify(notifyHandle.get(), 0);
}

/**
 * This is a callback function that will be called by spotify.
 **/
void SessionCallbacks::notifyMainThread(sp_session* session) {
  //effectively calls handleNotify in another thread
  uv_async_send(notifyHandle.get());
}

/**
 * async callback handle function for process events.
 * This function will always be called in the thread in which the sp_session was created.
 **/
static void handleNotify(uv_async_t* handle, int status) {
  uv_timer_stop(timer.get()); //a new timeout will be set at the end
  int nextTimeout = 0;
  while(nextTimeout == 0) {
    sp_session_process_events(application->session, &nextTimeout);
  }
  uv_timer_start(timer.get(), &handleTimeout, nextTimeout, 0);
}

void SessionCallbacks::metadata_updated(sp_session* session) {
  //If sp_session_player_load did not load the track it must be retried to play. Bug #26.
  if(Player::instance->isLoading) {
    Player::instance->retryPlay();
  }
  
  V8Utils::callV8FunctionWithNoArgumentsIfHandleNotEmpty(metadataUpdatedCallback);
}

void SessionCallbacks::loggedIn(sp_session* session, sp_error error) {
  if(SP_ERROR_OK != error) {
    std::cout << "Error logging in: " << sp_error_message(error) << std::endl;
    return;
  } else {
    std::cout << "Logged in" << std::endl;
  }

  //The creation of the root playlist container is absolutely necessary here, otherwise following callbacks can crash.
  rootPlaylistContainerCallbacks.container_loaded = &SessionCallbacks::rootPlaylistContainerLoaded;
  sp_playlistcontainer *pc = sp_session_playlistcontainer(application->session);
  application->playlistContainer = std::make_shared<PlaylistContainer>(pc);
  sp_playlistcontainer_add_callbacks(pc, &rootPlaylistContainerCallbacks, nullptr); 
}

/**
 * This is the "ready" hook for users. Playlists should be available at this point.
 **/
void SessionCallbacks::rootPlaylistContainerLoaded(sp_playlistcontainer* sp, void* userdata) {
  V8Utils::callV8FunctionWithNoArgumentsIfHandleNotEmpty(loginCallback);
  //Issue 35, rootPlaylistContainerLoaded can be called multiple times throughout the lifetime of a session.
  //loginCallback must only be called once.
  sp_playlistcontainer_remove_callbacks(sp, &rootPlaylistContainerCallbacks, nullptr);    
}

void SessionCallbacks::loggedOut(sp_session* session) {
  std::cout << "Logged out" << std::endl;
  V8Utils::callV8FunctionWithNoArgumentsIfHandleNotEmpty(logoutCallback);
}

void SessionCallbacks::end_of_track(sp_session* session) {
  sp_session_player_unload(application->session);
  spotify::framesReceived = 0;
  spotify::currentSecond = 0;
  
  V8Utils::callV8FunctionWithNoArgumentsIfHandleNotEmpty(endOfTrackCallback);
}

void SessionCallbacks::sendTimer(int sample_rate) {
  if( spotify::framesReceived / sample_rate > 0) {
    spotify::currentSecond++;
    spotify::framesReceived = spotify::framesReceived - sample_rate;
    Player::instance->setCurrentSecond(spotify::currentSecond);
  }
}

static void handleMusicInNodeThread(uv_async_t* handle, int status) {
  music* musicData = static_cast<music*>(handle->data);
  node::Buffer *slowBuffer = node::Buffer::New(musicData->size);
  memcpy(node::Buffer::Data(slowBuffer), musicData->data, musicData->size);
  v8::Local<v8::Object> globalObj = v8::Context::GetCurrent()->Global();
  v8::Local<v8::Function> ctor = v8::Local<v8::Function>::Cast(globalObj->Get(v8::String::New("Buffer")));
  v8::Handle<v8::Value> constructorArgs[3] = { slowBuffer->handle_, v8::Integer::New(musicData->size), v8::Integer::New(0)};
  v8::Handle<v8::Object> actualBuffer = ctor->NewInstance(3, constructorArgs);
  int argc = 2;
  v8::Handle<v8::Value> argv[] = { v8::Undefined(), actualBuffer };
  SessionCallbacks::musicDeliveryCallback->Call(globalObj, argc, argv);
  free(musicData->data);
  delete musicData;
}

/**
Sends the music data to the node thread via an async handle that will call handleMusicInNodeThread
**/
int SessionCallbacks::node_music_delivery(sp_session* session, const sp_audioformat* format, const void* frames, int num_frames) {
  music* musicData = new music();
  size_t size;
  size = num_frames * sizeof(int16_t) * format->channels;
  musicData->data = malloc(size);
  memcpy(musicData->data, frames, size);
  musicData->size = size;
  musicNotifyHandle.get()->data = (void*)musicData;
  uv_async_send(musicNotifyHandle.get());
  return num_frames;
}

int SessionCallbacks::music_delivery(sp_session *sess, const sp_audioformat *format,
                          const void *frames, int num_frames)
{
  audio_fifo_t *af = &application->audio_fifo;
  audio_fifo_data_t *afd;
  size_t s;

  if (num_frames == 0)
    return 0; // Audio discontinuity, do nothing

  pthread_mutex_lock(&af->mutex);

  // Buffer one second of audio
  if (af->qlen > format->sample_rate) {
    pthread_mutex_unlock(&af->mutex);
    return 0;
  }

  s = num_frames * sizeof(int16_t) * format->channels;

  afd = (audio_fifo_data_t*)malloc(sizeof(*afd) + s);
  memcpy(afd->samples, frames, s);

  afd->nsamples = num_frames;

  afd->rate = format->sample_rate;
  afd->channels = format->channels;

  TAILQ_INSERT_TAIL(&af->q, afd, link);
  af->qlen += num_frames;

  pthread_cond_signal(&af->cond);
  pthread_mutex_unlock(&af->mutex);

  spotify::framesReceived += num_frames;
  sendTimer(format->sample_rate);
  return num_frames;
}
