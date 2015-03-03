#ifndef _SPOTIFY_SERVICE_SESSION_CALLBACKS_H
#define _SPOTIFY_SERVICE_SESSION_CALLBACKS_H

#include <libspotify/api.h>
#include <memory>
#include <nan.h>

class SessionCallbacks {
public:
  static void notifyMainThread(sp_session* session);
  static void loggedIn(sp_session* session, sp_error error);
  static void loggedOut(sp_session* session);
  static void playTokenLost(sp_session* session);
  static void end_of_track(sp_session* session);
  static void init();
  static void metadata_updated(sp_session* session);
  static void start_playback(sp_session* session);
  static void stop_playback(sp_session* session);
  static void rootPlaylistContainerLoaded(sp_playlistcontainer* sp, void* userdata);
  static std::unique_ptr<NanCallback> loginCallback;
  static std::unique_ptr<NanCallback> logoutCallback;
  static std::unique_ptr<NanCallback> metadataUpdatedCallback;
  static std::unique_ptr<NanCallback> endOfTrackCallback;
  static std::unique_ptr<NanCallback> playTokenLostCallback;
};

#endif
