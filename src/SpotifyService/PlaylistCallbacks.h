#ifndef _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_H
#define _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_H

#include <libspotify/api.h>
#include <uv.h>

class PlaylistCallbacks {
public:
  static void playlistStateChanged(sp_playlist* _playlist, void* userdata);
  static void playlistNameChange(sp_playlist* spPlaylist, void* userdata);
  static void tracksAdded(sp_playlist* playlist, sp_track *const *tracks, int num_tracks, int position, void *userdata);
  static void tracks_moved(sp_playlist* playlist, const int *tracks, int num_tracks, int new_position, void *userdata);
  static void tracks_removed(sp_playlist* spPlaylist, const int *tracks, int num_tracks, void *userdata);
  /* staticvoid playlist_update_in_progress(sp_playlist *pl, bool done, void *userdata);
  static void track_created_changed(sp_playlist *pl, int position, sp_user *user, int when, void *userdata);*/
};

#endif
