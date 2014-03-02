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
#include "../../Application.h"
#include "../../exceptions.h"
#include "../../callbacks/PlaylistCallbacks.h"

extern Application* application;

Playlist::Playlist(sp_playlist* _playlist) : PlaylistBase(false), playlist(_playlist) {
  sp_playlist_add_callbacks(playlist, &Playlist::playlistCallbacks, nullptr);
  sp_playlist_add_ref(playlist);
}

Playlist::Playlist(const Playlist& other) : PlaylistBase(other.isFolder), playlist(other.playlist) {
  sp_playlist_add_ref(playlist);
}

Playlist::~Playlist() {
  sp_playlist_remove_callbacks(playlist, &Playlist::playlistCallbacks, nullptr);
  sp_playlist_release(playlist);
}

std::string Playlist::name() {
  std::string name;
  if(sp_playlist_is_loaded(playlist)) {
    name = std::string(sp_playlist_name(playlist));
  } else {
    name = std::string("Loading...");
  }
  return name;
}

void Playlist::name(std::string _name) {
  sp_playlist_rename(playlist, _name.c_str());
}

void Playlist::addTracks(std::vector<std::shared_ptr<Track>> tracks, int position) {
  sp_track* spTracks[tracks.size()];
  for(int i = 0; i < (int)tracks.size(); i++) {
    spTracks[i] = tracks[i]->track;
  }
  sp_playlist_add_tracks(playlist, spTracks, tracks.size(), position, application->session);
}

void Playlist::removeTracks(const int* trackPositions, int numberOfTracks) {
  sp_playlist_remove_tracks(playlist, trackPositions, numberOfTracks);
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

//Playlist::playlistCallbacks.tracks_moved = &PlaylistCallbacks::tracks_moved;
/*playlistCallbacks.playlist_update_in_progress = &playlist_update_in_progress;
playlistCallbacks.track_created_changed = &track_created_changed;*/
sp_playlist_callbacks Playlist::playlistCallbacks = {
  &PlaylistCallbacks::tracksAdded,
  &PlaylistCallbacks::tracksRemoved,
  nullptr,
  &PlaylistCallbacks::playlistNameChange,
  &PlaylistCallbacks::playlistStateChanged,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  nullptr
};