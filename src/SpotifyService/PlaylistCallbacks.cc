#include "PlaylistCallbacks.h"
#include "../objects/spotify/Playlist.h"
#include "../events.h"

#include <vector>
#include <pthread.h>

void PlaylistCallbacks::playlistNameChange(sp_playlist* _playlist, void* userdata) {
  Playlist* playlist = static_cast<Playlist*>(userdata);
  if(playlist->nodeObject != nullptr) {
    playlist->nodeObject->call(PLAYLIST_RENAMED);
  }
}

void PlaylistCallbacks::playlistStateChanged(sp_playlist* _playlist, void* userdata) {

}

/*void PlaylistCallbacks::tracks_added(sp_playlist* spPlaylist, sp_track *const *tracks, int num_tracks, int position, void *userdata) {
  Playlist* playlist  = static_cast<Playlist*>(userdata);

  if(playlist->tracksLoaded) {
    Track* newTracks[num_tracks];
    for(int i = 0; i < num_tracks; i++) {
      newTracks[i] = new Track(tracks[i]);
    }
    pthread_mutex_lock(&playlist->lockingMutex);
    playlist->tracks.insert(playlist->tracks.begin() + position, newTracks, newTracks + num_tracks);
    pthread_mutex_unlock(&playlist->lockingMutex);
    playlist->call(PLAYLIST_TRACKS_CHANGED);
  }
}

void PlaylistCallbacks::tracks_moved(sp_playlist* playlist, const int *tracks, int num_tracks, int new_position, void *userdata) {

}*/

/*void PlaylistCallbacks::tracks_removed(sp_playlist* spPlaylist, const int *tracks, int num_tracks, void *userdata) {
  Playlist* playlist  = static_cast<Playlist*>(userdata);
  if(playlist->tracksLoaded) {
    pthread_mutex_lock(&playlist->lockingMutex);
    auto it = playlist->tracks.begin();
    for(int i = 0; i < num_tracks; i++) {
      playlist->tracks.erase(it + tracks[i]);
    }
    pthread_mutex_unlock(&playlist->lockingMutex);
    playlist->call(PLAYLIST_TRACKS_CHANGED);
  }
}*/

/*void playlist_update_in_progress(sp_playlist *pl, bool done, void *userdata) {
  std::cout << "Update in progress" << std::endl;
}

void track_created_changed(sp_playlist *pl, int position, sp_user *user, int when, void *userdata) {
  std::cout << "Track created change" << std::endl;
}*/