#include "PlaylistContainer.h"
#include "../SpotifyService/PlaylistCallbacks.h"

Persistent<Function> PlaylistContainer::containerLoadedCallback;

//TODO: this can't be a local variable, but this here is shit too
sp_playlist_callbacks playlistCallbacks;

void PlaylistContainer::loadPlaylists() {
  int numPlaylists = sp_playlistcontainer_num_playlists(playlistContainer);

  playlistCallbacks.playlist_state_changed = &PlaylistCallbacks::playlistStateChanged;
  playlistCallbacks.playlist_renamed = &PlaylistCallbacks::playlistNameChange;
  playlistCallbacks.tracks_added = &PlaylistCallbacks::tracks_added;
  playlistCallbacks.tracks_moved = &PlaylistCallbacks::tracks_moved;
  /*playlistCallbacks.playlist_update_in_progress = &playlist_update_in_progress;
  playlistCallbacks.track_created_changed = &track_created_changed;*/

  for(int i = 0; i < numPlaylists; ++i) {
    sp_playlist* spPlaylist = sp_playlistcontainer_playlist(playlistContainer, i);
    Playlist* playlist = new Playlist(spPlaylist, i);
    sp_playlist_add_callbacks(spPlaylist, &playlistCallbacks, playlist);
    playlists.push_back(playlist);
  }
  sp_playlist* spPlaylist = sp_session_starred_create(spotifyService->spotifySession);
  Playlist* playlist = new Playlist(spPlaylist, numPlaylists);
  sp_playlist_add_callbacks(spPlaylist, &playlistCallbacks, playlist);
  playlists.push_back(playlist);
}
