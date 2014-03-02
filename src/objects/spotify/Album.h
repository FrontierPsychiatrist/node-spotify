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

#ifndef _ALBUM_H
#define _ALBUM_H

#include "Track.h"
#include "Artist.h"
#include "../node/V8Browseable.h"

#include <libspotify/api.h>
#include <string>
#include <vector>
#include <memory>

class Track;
class Artist;

class Album {
friend class NodeAlbum;
friend class AlbumBrowseCallbacks;
public:
  Album(sp_album* _album);
  ~Album();
  Album(const Album& other);
  std::string name();
  std::string link();
  std::string coverBase64();
  std::vector<std::shared_ptr<Track>> tracks();
  std::string review();
  std::vector<std::string> copyrights();
  std::shared_ptr<Artist> artist();
  void browse();
  bool isLoaded();
private:
  sp_album* album;
  sp_image* cover;
  V8Browseable* nodeObject;
  sp_albumbrowse* albumBrowse;
};

#endif