#ifndef _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_H
#define _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_H

#include <libspotify/api.h>
#include <uv.h>

namespace spotify {

void playlistStateChanged(sp_playlist* _playlist, void* userdata);
void playlistNameChange(sp_playlist* spPlaylist, void* userdata);
void tracks_added(sp_playlist* playlist, sp_track *const *tracks, int num_tracks, int position, void *userdata);
void tracks_moved(sp_playlist* playlist, const int *tracks, int num_tracks, int new_position, void *userdata);
/*void playlist_update_in_progress(sp_playlist *pl, bool done, void *userdata);
void track_created_changed(sp_playlist *pl, int position, sp_user *user, int when, void *userdata);*/
}

#endif
