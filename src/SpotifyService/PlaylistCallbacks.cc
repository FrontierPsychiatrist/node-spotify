#include "PlaylistCallbacks.h"
#include "../objects/spotify/Playlist.h"
#include "../events.h"

#include <vector>

void PlaylistCallbacks::playlistNameChange(sp_playlist* spPlaylist, void* userdata) {
  Playlist* playlist = static_cast<Playlist*>(userdata);
  playlist->name = std::string(sp_playlist_name(spPlaylist));
}

void PlaylistCallbacks::playlistStateChanged(sp_playlist* _playlist, void* userdata) {
  Playlist* playlist = static_cast<Playlist*>(userdata);
}

/*void PlaylistCallbacks::tracks_added(sp_playlist* spPlaylist, sp_track *const *tracks, int num_tracks, int position, void *userdata) {
  Playlist* playlist  = static_cast<Playlist*>(userdata);

  if(playlist->tracksLoaded) {
    Track* newTracks[num_tracks];
    for(int i = 0; i < num_tracks; i++) {
      newTracks[i] = new Track(tracks[i]);
    }

    playlist->tracks.insert(playlist->tracks.begin() + position, newTracks, newTracks + num_tracks);
  }
}

void PlaylistCallbacks::tracks_moved(sp_playlist* playlist, const int *tracks, int num_tracks, int new_position, void *userdata) {
  
}*/

/*void playlist_update_in_progress(sp_playlist *pl, bool done, void *userdata) {
  std::cout << "Update in progress" << std::endl; 
}

void track_created_changed(sp_playlist *pl, int position, sp_user *user, int when, void *userdata) {
  std::cout << "Track created change" << std::endl;  
}*/