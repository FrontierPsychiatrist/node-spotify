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

#ifndef _SEARCH_RESULT_H
#define _SEARCH_RESULT_H

#include <memory>
#include <vector>
#include <string>

#include "Track.h"
#include "Album.h"
#include "Playlist.h"
#include "Artist.h"

#include "../node/V8Browseable.h"

class Search {
friend class NodeSearch;
friend class SearchCallbacks;
public:
  Search() {};
  Search(const Search& other);
  ~Search();
  std::vector<std::shared_ptr<Track>> getTracks();
  std::vector<std::shared_ptr<Album>> getAlbums();
  std::vector<std::shared_ptr<Artist>> getArtists();
  std::vector<std::shared_ptr<Playlist>> getPlaylists();
  void execute(std::string query, int trackOffset, int trackLimit,
    int albumOffset, int albumLimit,
    int artistOffset, int artistLimit,
    int playlistOffset, int playlistLimit);
  std::string link();
  std::string didYouMeanText();
  int totalTracks();
  int totalAlbums();
  int totalArtists();
  int totalPlaylists();
private:
  sp_search* search;
  V8Browseable* nodeObject;
};

#endif