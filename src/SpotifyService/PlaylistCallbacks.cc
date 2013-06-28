#include "PlaylistCallbacks.h"
#include "../spotify/Playlist.h"
#include "../events.h"

void PlaylistCallbacks::playlistNameChange(sp_playlist* spPlaylist, void* userdata) {
  Playlist* playlist = static_cast<Playlist*>(userdata);
  playlist->setName(std::string(sp_playlist_name(spPlaylist)));
}

void PlaylistCallbacks::playlistStateChanged(sp_playlist* _playlist, void* userdata) {
  Playlist* playlist = static_cast<Playlist*>(userdata);
  if(sp_playlist_is_loaded(_playlist)) {
    playlist->setName(std::string(sp_playlist_name(_playlist)));
  }
}

void PlaylistCallbacks::tracks_added(sp_playlist* playlist, sp_track *const *tracks, int num_tracks, int position, void *userdata) {
  
}

void PlaylistCallbacks::tracks_moved(sp_playlist* playlist, const int *tracks, int num_tracks, int new_position, void *userdata) {
  
}

/*void playlist_update_in_progress(sp_playlist *pl, bool done, void *userdata) {
  std::cout << "Update in progress" << std::endl; 
}

void track_created_changed(sp_playlist *pl, int position, sp_user *user, int when, void *userdata) {
  std::cout << "Track created change" << std::endl;  
}*/