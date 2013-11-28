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

#include "Playlist.h"
#include <future>
#include "../../Application.h"

extern Application* application;

Playlist::Playlist(sp_playlist* _playlist) : playlist(_playlist), nodeObject(nullptr) {
  sp_playlist_add_ref(playlist);
};

std::string Playlist::name() {
  std::string name;
  if(sp_playlist_is_loaded(playlist)) {
    name = std::string(sp_playlist_name(playlist));
  } else {
    name = std::string("Loading...");
  }
  return name;
}

std::string Playlist::link() {
  std::string link;
  if(sp_playlist_is_loaded(playlist)) {
    sp_link* spLink = sp_link_create_from_playlist(playlist);
    if(spLink != nullptr) {
      char linkChar[256];
      sp_link_as_string(spLink, linkChar, 256);
      link = std::string(linkChar);
      sp_link_release(spLink);
    }
  }
  return link;
}

bool Playlist::isLoaded() {
  return sp_playlist_is_loaded(playlist);
}

std::vector<std::shared_ptr<Track>> Playlist::getTracks() {
  std::vector<std::shared_ptr<Track>> tracks(sp_playlist_num_tracks(playlist));
  for(int i = 0; i < (int)tracks.size(); ++i) {
    tracks[i] = std::make_shared<Track>(sp_playlist_track(playlist, i));
  }
  return tracks;
}

sp_playlist_callbacks Playlist::playlistCallbacks;