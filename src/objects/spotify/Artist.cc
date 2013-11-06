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

#include "Artist.h"
#include "../../callbacks/ArtistBrowseCallbacks.h"
#include "../../Application.h"

extern Application* application;

Artist::Artist(sp_artist* _artist) : artist(_artist), artistBrowse(nullptr), nodeObject(nullptr) {
  sp_artist_add_ref(artist);
};

 Artist::Artist(const Artist& other) : artist(other.artist), artistBrowse(other.artistBrowse), nodeObject(other.nodeObject) {
  sp_artist_add_ref(artist);
  if(artistBrowse != nullptr) {
    sp_artistbrowse_add_ref(artistBrowse);
  }
};

Artist::~Artist() {
  sp_artist_release(artist);
  if(artistBrowse != nullptr) {
    sp_artistbrowse_release(artistBrowse);
  }
};

std::string Artist::name() {
  std::string name;
  if(sp_artist_is_loaded(artist)) {
    name = std::string(sp_artist_name(artist));
  } else {
    name = "Loading...";
  }
  return name;
}

std::string Artist::link() {
  std::string link;
  if(sp_artist_is_loaded(artist)) {
    sp_link* spLink = sp_link_create_from_artist(artist);
    char linkChar[256];
    sp_link_as_string(spLink, linkChar, 256);
    link = std::string(linkChar);
    sp_link_release(spLink);
  }
  return link;
}

void Artist::browse(sp_artistbrowse_type artistbrowseType) {
  this->artistBrowse = sp_artistbrowse_create(application->session, artist, artistbrowseType, &ArtistBrowseCallbacks::artistBrowseComplete, this);
}

std::vector<std::shared_ptr<Track>> Artist::tracks() {
  std::vector<std::shared_ptr<Track>> tracks;
  if(sp_artistbrowse_is_loaded(artistBrowse)) {
    int numTracks = sp_artistbrowse_num_tracks(artistBrowse);
    tracks.resize(numTracks);
    for(int i = 0; i < numTracks; i++) {
      tracks[i] = std::make_shared<Track>(sp_artistbrowse_track(artistBrowse, i));
    }
  }
  return tracks;
}

std::vector<std::shared_ptr<Track>> Artist::tophitTracks() {
  std::vector<std::shared_ptr<Track>> tophitTracks;
  if(sp_artistbrowse_is_loaded(artistBrowse)) {
    int numTophitTracks = sp_artistbrowse_num_tophit_tracks(artistBrowse);
    tophitTracks.resize(numTophitTracks);
    for(int i = 0; i < numTophitTracks; i++) {
      tophitTracks[i] = std::make_shared<Track>(sp_artistbrowse_tophit_track(artistBrowse, i));
    }
  }
  return tophitTracks;
}

std::vector<std::shared_ptr<Album>> Artist::albums() {
  std::vector<std::shared_ptr<Album>> albums;
  if(sp_artistbrowse_is_loaded(artistBrowse)) {
    int numAlbums = sp_artistbrowse_num_albums(artistBrowse);
    albums.resize(numAlbums);
    for(int i = 0; i < numAlbums; i++) {
      albums[i] = std::make_shared<Album>(sp_artistbrowse_album(artistBrowse, i));
    }
  }
  return albums;
}

std::vector<std::shared_ptr<Artist>> Artist::similarArtists() {
  std::vector<std::shared_ptr<Artist>> similarArtists;
  if(sp_artistbrowse_is_loaded(artistBrowse)) {
    int numSimilarArtists = sp_artistbrowse_num_similar_artists(artistBrowse);
    similarArtists.resize(numSimilarArtists);
    for(int i = 0; i < numSimilarArtists; i++) {
      similarArtists[i] = std::make_shared<Artist>(sp_artistbrowse_similar_artist(artistBrowse, i));
    }
  }
  return similarArtists;
}

std::string Artist::biography() {
  std::string biography;
  if(sp_artistbrowse_is_loaded(artistBrowse)) {
    biography = std::string(sp_artistbrowse_biography(artistBrowse));
  }
  return biography;
}