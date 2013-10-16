#include "PlaylistContainer.h"
#include "../../SpotifyService/PlaylistCallbacks.h"
#include "../../Application.h"
#include "StarredPlaylist.h"

extern Application* application;

void PlaylistContainer::loadPlaylists() {
  int numPlaylists = sp_playlistcontainer_num_playlists(playlistContainer);
  playlists.resize(numPlaylists);

  Playlist::playlistCallbacks.playlist_state_changed = &PlaylistCallbacks::playlistStateChanged;
  Playlist::playlistCallbacks.playlist_renamed = &PlaylistCallbacks::playlistNameChange;
  //Playlist::playlistCallbacks.tracks_added = &PlaylistCallbacks::tracks_added;
  //Playlist::playlistCallbacks.tracks_moved = &PlaylistCallbacks::tracks_moved;
  /*playlistCallbacks.playlist_update_in_progress = &playlist_update_in_progress;
  playlistCallbacks.track_created_changed = &track_created_changed;*/

  for(int i = 0; i < numPlaylists; ++i) {
    sp_playlist* spPlaylist = sp_playlistcontainer_playlist(playlistContainer, i);
    playlists[i] = std::make_shared<Playlist>(spPlaylist, i);
    sp_playlist_add_callbacks(spPlaylist, &Playlist::playlistCallbacks, playlists[i].get());
  }

  sp_playlist* spPlaylist = sp_session_starred_create(application->session);
  starredPlaylist = std::make_shared<StarredPlaylist>(spPlaylist, numPlaylists);
  sp_playlist_add_callbacks(spPlaylist, &Playlist::playlistCallbacks, starredPlaylist.get());
}
