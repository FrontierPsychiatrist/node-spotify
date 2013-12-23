/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#include "SessionCallbacks.h"

#include "../Application.h"

#include "../objects/spotify/PlaylistContainer.h"
#include "../objects/spotify/Player.h"
#include "../events.h"

extern "C" {
  #include "../audio/audio.h"
}

#include <stdlib.h>
#include <string.h>
#include <iostream>

extern Application* application;

static sp_playlistcontainer_callbacks rootPlaylistContainerCallbacks;

std::unique_ptr<uv_timer_t> SessionCallbacks::timer;
std::unique_ptr<uv_async_t> SessionCallbacks::notifyHandle;
v8::Handle<v8::Function> SessionCallbacks::loginCallback;

namespace spotify {
//TODO
int framesReceived = 0;
int currentSecond = 0;
}

void SessionCallbacks::init() {
  timer = std::unique_ptr<uv_timer_t>(new uv_timer_t());
  notifyHandle = std::unique_ptr<uv_async_t>(new uv_async_t());
  uv_async_init(uv_default_loop(), notifyHandle.get(), handleNotify);
  uv_timer_init(uv_default_loop(), timer.get());
}

/**
 * If the timer has run out this method will be called.
 **/
void SessionCallbacks::processEvents(uv_timer_t* timer, int status) {
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
void SessionCallbacks::handleNotify(uv_async_t* handle, int status) {
  uv_timer_stop(timer.get()); //a new timeout will be set at the end
  int nextTimeout = 0;
  while(nextTimeout == 0) {
    sp_session_process_events(application->session, &nextTimeout);
  }
  uv_timer_start(timer.get(), &processEvents, nextTimeout, 0);
}

void SessionCallbacks::metadata_updated(sp_session* session) {
  //If sp_session_player_load did not load the track it must be retried to play. Bug #26.
  if(Player::instance->isLoading) {
    Player::instance->retryPlay();
  }
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

  //Trigger the login complete callback
  if(!loginCallback.IsEmpty()) {
    unsigned int argc = 0;
    v8::Handle<v8::Value> argv[0];
    loginCallback->Call(v8::Context::GetCurrent()->Global(), argc, argv);
  }
}

void SessionCallbacks::loggedOut(sp_session* session) {
  std::cout << "Logged out" << std::endl;
}

void SessionCallbacks::rootPlaylistContainerLoaded(sp_playlistcontainer* spPlaylistContainer, void* userdata) {

}

void SessionCallbacks::end_of_track(sp_session* session) {
  sp_session_player_unload(application->session);
  spotify::framesReceived = 0;
  spotify::currentSecond = 0;
  if(Player::instance->nodeObject != nullptr) {
    Player::instance->nodeObject->call(PLAYER_END_OF_TRACK);
  }
}

void SessionCallbacks::sendTimer(int sample_rate) {
  if( spotify::framesReceived / sample_rate > 0) {
    spotify::currentSecond++;
    spotify::framesReceived = spotify::framesReceived - sample_rate;
    Player::instance->setCurrentSecond(spotify::currentSecond);
  }
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