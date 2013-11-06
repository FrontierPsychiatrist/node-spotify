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

#ifndef _ARTIST_H
#define _ARTIST_H

#include <string>
#include <vector>
#include <memory>
#include <libspotify/api.h>

#include "Track.h"
#include "Album.h"
#include "../node/V8Callable.h"

class Track;
class Album;

class Artist {
friend class NodeArtist;
friend class ArtistBrowseCallbacks;
public:
  Artist(sp_artist* _artist);
  Artist(const Artist& other);
  ~Artist();

  std::string name();
  std::string link();
  std::vector<std::shared_ptr<Track>> tracks();
  std::vector<std::shared_ptr<Track>> tophitTracks();
  std::vector<std::shared_ptr<Album>> albums();
  std::vector<std::shared_ptr<Artist>> similarArtists();
  std::string biography();
  void browse(sp_artistbrowse_type artistbrowseType);
private:
  sp_artist* artist;
  sp_artistbrowse* artistBrowse;
  V8Callable* nodeObject;
};

#endif