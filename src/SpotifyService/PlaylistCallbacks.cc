#include "PlaylistCallbacks.h"
#include "../objects/spotify/Track.h"
#include "../objects/node/NodeTrack.h"
#include "../objects/spotify/Playlist.h"
#include "../events.h"

#include <v8.h>
#include <memory>

void PlaylistCallbacks::playlistNameChange(sp_playlist* _playlist, void* userdata) {
  Playlist* playlist = static_cast<Playlist*>(userdata);
  if(playlist->nodeObject != nullptr) {
    playlist->nodeObject->call(PLAYLIST_RENAMED);
  }
}

void PlaylistCallbacks::playlistStateChanged(sp_playlist* _playlist, void* userdata) {

}

void PlaylistCallbacks::tracksAdded(sp_playlist* spPlaylist, sp_track *const *tracks, int num_tracks, int position, void *userdata) {
  Playlist* playlist  = static_cast<Playlist*>(userdata);
  v8::HandleScope scope;
  v8::Handle<v8::Array> nodeTracks = v8::Array::New(num_tracks);
  for(int i = 0; i < num_tracks; i++) {
    NodeTrack* nodeTrack = new NodeTrack(std::make_shared<Track>(tracks[i]));
    nodeTracks->Set(v8::Number::New(i), nodeTrack->getV8Object());
  }
  playlist->nodeObject->call(PLAYLIST_TRACKS_ADDED, nodeTracks);
  scope.Close(Undefined());
}

/*void PlaylistCallbacks::tracks_moved(sp_playlist* playlist, const int *tracks, int num_tracks, int new_position, void *userdata) {

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