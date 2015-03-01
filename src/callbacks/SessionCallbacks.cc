#include "SessionCallbacks.h"
#include "../Application.h"
#include "../objects/spotify/PlaylistContainer.h"
#include "../objects/spotify/Player.h"

#include <string.h>
#include <uv.h>
#include <node_version.h>

extern Application* application;

#if NODE_VERSION_AT_LEAST(0, 11, 0)
static void handleNotify(uv_async_t* handle);
#else
static void handleNotify(uv_async_t* handle, int status);
#endif

static sp_playlistcontainer_callbacks rootPlaylistContainerCallbacks;
//Timer to call sp_session_process_events after a timeout
static std::unique_ptr<uv_timer_t> processEventsTimer;
static std::unique_ptr<uv_async_t> notifyHandle;

std::unique_ptr<NanCallback> SessionCallbacks::loginCallback;
std::unique_ptr<NanCallback> SessionCallbacks::logoutCallback;
std::unique_ptr<NanCallback> SessionCallbacks::metadataUpdatedCallback;
std::unique_ptr<NanCallback> SessionCallbacks::endOfTrackCallback;
std::unique_ptr<NanCallback> SessionCallbacks::playTokenLostCallback;

void SessionCallbacks::init() {
  processEventsTimer = std::unique_ptr<uv_timer_t>(new uv_timer_t());
  notifyHandle = std::unique_ptr<uv_async_t>(new uv_async_t());
  uv_async_init(uv_default_loop(), notifyHandle.get(), handleNotify);
  uv_timer_init(uv_default_loop(), processEventsTimer.get());
}

/**
 * If the timer for sp_session_process_events has run out this method will be called.
 **/
#if NODE_VERSION_AT_LEAST(0, 11, 0)
static void processEventsTimeout(uv_timer_t* timer) {
  handleNotify(notifyHandle.get());
}
#else
static void processEventsTimeout(uv_timer_t* timer, int status) {
  handleNotify(notifyHandle.get(), 0);
}
#endif


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
#if NODE_VERSION_AT_LEAST(0, 11, 0)
static void handleNotify(uv_async_t* handle) {
#else
static void handleNotify(uv_async_t* handle, int status) {
#endif
  uv_timer_stop(processEventsTimer.get()); //a new timeout will be set at the end
  int nextTimeout = 0;
  while(nextTimeout == 0) {
    sp_session_process_events(application->session, &nextTimeout);
  }
  uv_timer_start(processEventsTimer.get(), &processEventsTimeout, nextTimeout, 0);
}

void SessionCallbacks::metadata_updated(sp_session* session) {
  //If sp_session_player_load did not load the track it must be retried to play. Bug #26.
  if(application->player->isLoading) {
    application->player->retryPlay();
  }
  
  if(metadataUpdatedCallback && !metadataUpdatedCallback->IsEmpty()) {
    metadataUpdatedCallback->Call(0, {});
  }
}

void SessionCallbacks::loggedIn(sp_session* session, sp_error error) {
  if(SP_ERROR_OK != error) {
    unsigned int argc = 1;
    v8::Handle<v8::Value> argv[1] = { NanError(sp_error_message(error)) };
    loginCallback->Call( argc, argv );
    return;
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
  if(loginCallback && !loginCallback->IsEmpty()) {
    loginCallback->Call(0, {});
  }
  //Issue 35, rootPlaylistContainerLoaded can be called multiple times throughout the lifetime of a session.
  //loginCallback must only be called once.
  sp_playlistcontainer_remove_callbacks(sp, &rootPlaylistContainerCallbacks, nullptr);    
}

void SessionCallbacks::playTokenLost(sp_session *session) {
  application->audioHandler->setStopped(true);
  application->player->isPaused = true;
  if(playTokenLostCallback && !playTokenLostCallback->IsEmpty()) {
    playTokenLostCallback->Call(0, {});
  }
}

void SessionCallbacks::loggedOut(sp_session* session) {
  if(logoutCallback && !logoutCallback->IsEmpty()) {
    logoutCallback->Call(0, {});
  }
}
