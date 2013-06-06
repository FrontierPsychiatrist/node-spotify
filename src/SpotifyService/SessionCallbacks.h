#ifndef _SPOTIFY_SERVICE_SESSION_CALLBACKS_H
#define _SPOTIFY_SERVICE_SESSION_CALLBACKS_H

#include <libspotify/api.h>
#include <uv.h>

namespace spotify {

void notifyMainThread(sp_session* session); 
void loggedIn(sp_session* session, sp_error error);
void loggedOut(sp_session* session);
void rootPlaylistContainerLoaded(sp_playlistcontainer* spPlaylistContainer, void* userdata);
int music_delivery(sp_session *sess, const sp_audioformat *format, const void *frames, int num_frames);

}
#endif
