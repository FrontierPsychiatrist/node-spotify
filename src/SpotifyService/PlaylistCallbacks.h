#ifndef _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_H
#define _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_H

#include <libspotify/api.h>
#include <uv.h>

namespace spotify {

void playlistStateChanged(sp_playlist* _playlist, void* userdata);
void playlistNameChange(sp_playlist* spPlaylist, void* userdata);

}

#endif
