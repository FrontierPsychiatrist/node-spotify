#ifndef _SPOTIFY_SERVICE_SESSION_CALLBACKS_H
#define _SPOTIFY_SERVICE_SESSION_CALLBACKS_H

#include <libspotify/api.h>
#include <uv.h>
#include <memory>
#include <v8.h>

class SessionCallbacks {
public:
  static void notifyMainThread(sp_session* session);
  static void loggedIn(sp_session* session, sp_error error);
  static void loggedOut(sp_session* session);
  static int music_delivery(sp_session *sess, const sp_audioformat *format, const void *frames, int num_frames);
  static int node_music_delivery(sp_session *sess, const sp_audioformat *format, const void *frames, int num_frames);
  static void end_of_track(sp_session* session);
  static void init();
  static void metadata_updated(sp_session* session);
  static void rootPlaylistContainerLoaded(sp_playlistcontainer* sp, void* userdata);
  static v8::Handle<v8::Function> loginCallback;
  static v8::Handle<v8::Function> logoutCallback;
  static v8::Handle<v8::Function> metadataUpdatedCallback;
  static v8::Handle<v8::Function> endOfTrackCallback;
  static v8::Handle<v8::Function> musicDeliveryCallback;
private:
  static void sendTimer(int sample_rate);
};

#endif
