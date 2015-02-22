/**
Everything audio related in Session Callbacks.
**/
#include "SessionCallbacks.h"
#include "../Application.h"

extern Application* application;

void SessionCallbacks::end_of_track(sp_session* session) {
  sp_session_player_unload(application->session);

  if(endOfTrackCallback && !endOfTrackCallback->IsEmpty()) {
    endOfTrackCallback->Call(0, {});
  }
}

void SessionCallbacks::start_playback(sp_session* session) {
}

void SessionCallbacks::stop_playback(sp_session* session) {

}
