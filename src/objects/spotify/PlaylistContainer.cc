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
#include "../../Application.h"
#include "../../exceptions.h"
#include "StarredPlaylist.h"
#include "PlaylistFolder.h"

extern Application* application;

std::vector<std::shared_ptr<PlaylistBase>> PlaylistContainer::getPlaylists() {
  int numPlaylists = sp_playlistcontainer_num_playlists(playlistContainer);
  auto playlists = std::vector<std::shared_ptr<PlaylistBase>>((numPlaylists));
  for(int i = 0; i < numPlaylists; ++i) {
    sp_playlist_type playlistType = sp_playlistcontainer_playlist_type(playlistContainer, i);
    if(playlistType == SP_PLAYLIST_TYPE_PLAYLIST) {
      sp_playlist* spPlaylist = sp_playlistcontainer_playlist(playlistContainer, i);
      playlists[i] = std::make_shared<Playlist>(spPlaylist);
    } else if(playlistType == SP_PLAYLIST_TYPE_START_FOLDER) {
      char buf[256];
      sp_playlistcontainer_playlist_folder_name(playlistContainer, i, buf, 256);
      playlists[i] = std::make_shared<PlaylistFolder>(buf);
    } else if(playlistType == SP_PLAYLIST_TYPE_END_FOLDER) {
      playlists[i] = std::make_shared<PlaylistFolder>();
    }
  }
  return playlists;
}

std::shared_ptr<Playlist> PlaylistContainer::starredPlaylist() {
  sp_playlist* spPlaylist = sp_session_starred_create(application->session);
  return std::make_shared<Playlist>(spPlaylist);
}

void PlaylistContainer::addPlaylist(std::string name) {
  sp_playlist* spotifyPlaylist = sp_playlistcontainer_add_new_playlist(playlistContainer, name.c_str());
  if(spotifyPlaylist == nullptr) {
    throw PlaylistCreationException();
  }
}

void PlaylistContainer::removePlaylist(int position) {
  sp_playlistcontainer_remove_playlist(playlistContainer, position);
}
