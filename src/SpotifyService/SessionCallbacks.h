#ifndef _SPOTIFY_SERVICE_SESSION_CALLBACKS_H
#define _SPOTIFY_SERVICE_SESSION_CALLBACKS_H

#include <libspotify/api.h>

class SessionCallbacks {
public:
  static void notifyMainThread(sp_session* session); 
  static void loggedIn(sp_session* session, sp_error error);
  static void loggedOut(sp_session* session);
  static void rootPlaylistContainerLoaded(sp_playlistcontainer* spPlaylistContainer, void* userdata);
  static int music_delivery(sp_session *sess, const sp_audioformat *format, const void *frames, int num_frames);
  static void end_of_track(sp_session* session);
};

#endif
