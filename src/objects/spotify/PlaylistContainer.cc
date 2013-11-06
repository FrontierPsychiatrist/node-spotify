/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#include "PlaylistContainer.h"
#include "../../callbacks/PlaylistCallbacks.h"
#include "../../Application.h"
#include "StarredPlaylist.h"

extern Application* application;

void PlaylistContainer::loadPlaylists() {
  int numPlaylists = sp_playlistcontainer_num_playlists(playlistContainer);
  playlists.resize(numPlaylists);

  Playlist::playlistCallbacks.playlist_state_changed = &PlaylistCallbacks::playlistStateChanged;
  Playlist::playlistCallbacks.playlist_renamed = &PlaylistCallbacks::playlistNameChange;
  Playlist::playlistCallbacks.tracks_added = &PlaylistCallbacks::tracksAdded;
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
