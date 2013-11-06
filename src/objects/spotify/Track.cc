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

#include "Track.h"
#include "../../Application.h"

extern Application* application;

Track::Track(sp_track* _track) : track(_track) {
  sp_track_add_ref(track);
};

std::string Track::name() {
  std::string name;
  if(sp_track_is_loaded(track)) {
    name = std::string(sp_track_name(track));
  } else {
    name = "Loading...";
  }
  return name;
}

std::string Track::link() {
  std::string link;
  if(sp_track_is_loaded(track)) {
    sp_link* spLink = sp_link_create_from_track(track, 0);
    char linkChar[256];
    sp_link_as_string(spLink, linkChar, 256);
    link = std::string(linkChar);
    sp_link_release(spLink);
  }
  return link;
}

std::vector<std::shared_ptr<Artist>> Track::artists() {
  std::vector<std::shared_ptr<Artist>> artists;
  if(sp_track_is_loaded(track)) {
    int numArtists = sp_track_num_artists(track);
    artists.resize(numArtists);
    for(int i = 0; i < numArtists; i++) {
      sp_artist* spArtist = sp_track_artist(track, i);
      artists[i] = std::make_shared<Artist>(spArtist);
    }
  }
  return artists;
}

std::shared_ptr<Album> Track::album() {
  std::shared_ptr<Album> album;
  if(sp_track_is_loaded(track)) {
    sp_album* spAlbum = sp_track_album(track);
    album = std::make_shared<Album>(spAlbum);
  }
  return album;
}

int Track::duration() {
  int duration = -1;
  if(sp_track_is_loaded(track)) {
    duration = sp_track_duration(track);
  }
  return duration;
}

bool Track::starred() {
  bool starred = false;
  if(sp_track_is_loaded(track)) {
    starred = sp_track_is_starred(application->session, track);
  }
  return starred;
}

int Track::popularity() {
  int popularity = -1;
  if(sp_track_is_loaded(track)) {
    popularity = sp_track_popularity(track);
  }
  return popularity;
}

void Track::setStarred(bool starred) {
  //This takes an array of pointers to nodeTracks, so we need to tack the adress of the saved spotifyNodeTrack pointer.
  sp_track_set_starred(application->session, &track, 1, starred);
}