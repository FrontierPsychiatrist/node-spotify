#include "PlaylistContainer.h"
#include "../SpotifyService/PlaylistCallbacks.h"

using namespace spotify;

Persistent<Function> PlaylistContainer::containerLoadedCallback;

void PlaylistContainer::loadPlaylists() {
  int numPlaylists = sp_playlistcontainer_num_playlists(spPlaylistContainer);

  playlistCallbacks.playlist_state_changed = &playlistStateChanged;
  playlistCallbacks.playlist_renamed = &playlistNameChange;

  for(int i = 0; i < numPlaylists; ++i) {
    sp_playlist* spPlaylist = sp_playlistcontainer_playlist(spPlaylistContainer, highestPlaylistId);
    Playlist* playlist = new Playlist(spPlaylist, &spotifyService->callNodeThread, i);
    sp_playlist_add_callbacks(spPlaylist, &playlistCallbacks, playlist);
    playlists->push(playlist);
    highestPlaylistId++;
  }
}