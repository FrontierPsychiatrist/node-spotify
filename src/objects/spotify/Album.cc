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

#include "Album.h"
#include "../../utils/ImageUtils.h"
#include "../../Application.h"
#include "../../callbacks/AlbumBrowseCallbacks.h"

extern Application* application;

Album::Album(sp_album* _album) : album(_album), cover(nullptr), nodeObject(nullptr), albumBrowse(nullptr) {
  sp_album_add_ref(album);
};

Album::Album(const Album& other) : album(other.album), cover(other.cover) {
  sp_album_add_ref(album);
  if(cover != nullptr) {
    sp_image_add_ref(cover);
  }
  if(albumBrowse != nullptr) {
    sp_albumbrowse_add_ref(albumBrowse);
  }
};

Album::~Album() {
  sp_album_release(album);
  if(cover != nullptr) {
    sp_image_release(cover);
  }
  if(albumBrowse != nullptr) {
    sp_albumbrowse_release(albumBrowse);
  }
};

std::string Album::name() {
  std::string name;
  if(sp_album_is_loaded(album)) {
    name = std::string(sp_album_name(album));
  } else {
    name = "Loading...";
  }
  return name;
}

std::string Album::link() {
  std::string link;
  if(sp_album_is_loaded(album)) {
    char linkChar[256];
    sp_link* spLink = sp_link_create_from_album(album);
    sp_link_as_string(spLink, linkChar, 256);
    link = std::string(linkChar);
    sp_link_release(spLink);
  }
  return link;
}

std::vector<std::shared_ptr<Track>> Album::tracks() {
  std::vector<std::shared_ptr<Track>> tracks;
  if(sp_albumbrowse_is_loaded(albumBrowse)) {
    int numTracks = sp_albumbrowse_num_tracks(albumBrowse);
    tracks.resize(numTracks);
    for(int i = 0; i < numTracks; i++) {
      tracks[i] = std::make_shared<Track>(sp_albumbrowse_track(albumBrowse, i));
    }
  }
  return tracks;
}

std::string Album::review() {
  std::string review;
  if(sp_albumbrowse_is_loaded(albumBrowse)) {
    review = std::string(sp_albumbrowse_review(albumBrowse));
  }
  return review;
}

std::vector<std::string> Album::copyrights() {
  std::vector<std::string> copyrights;
  if(sp_albumbrowse_is_loaded(albumBrowse)) {
    int numCopyrights = sp_albumbrowse_num_copyrights(albumBrowse);
    copyrights.resize(numCopyrights);
    for(int i = 0; i < numCopyrights; i++) {
      copyrights[i] = std::string(sp_albumbrowse_copyright(albumBrowse, i));
    }
  }
  return copyrights;
}

std::shared_ptr<Artist> Album::artist() {
  std::shared_ptr<Artist> artist;
  if(sp_albumbrowse_is_loaded(albumBrowse)) {
    artist = std::make_shared<Artist>(sp_albumbrowse_artist(albumBrowse));
  }
  return artist;
}

std::string Album::coverBase64() {
  std::string cover;
  if(sp_album_is_loaded(album)) {
    const byte* coverId = sp_album_cover(album, SP_IMAGE_SIZE_NORMAL);
    if(coverId != nullptr) {
      sp_image* image = sp_image_create(application->session, coverId);
      if(sp_image_is_loaded(image)) {
        this->cover = image;
        cover = std::string(ImageUtils::convertImageToBase64(image));
      } else {
        cover = "";
        sp_image_add_load_callback(image, ImageUtils::imageLoadedCallback, nullptr);
      }
    }
  } else {
    cover = "";
  }
  return cover;
}

void Album::browse() {
  this->albumBrowse = sp_albumbrowse_create(application->session, album, AlbumBrowseCallbacks::albumBrowseComplete, this);
}

bool Album::isLoaded() {
  return sp_album_is_loaded(album);
}