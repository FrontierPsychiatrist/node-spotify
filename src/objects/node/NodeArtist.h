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

#ifndef _NODE_ARTIST_H
#define _NODE_ARTIST_H

#include <memory>
#include "NodeWrappedWithCallbacks.h"
#include "../spotify/Artist.h"

using namespace v8;

class NodeArtist : public NodeWrappedWithCallbacks<NodeArtist> {
private:
  std::shared_ptr<Artist> artist;
public:
  NodeArtist(std::shared_ptr<Artist> _artist) : artist(_artist) {
    artist->nodeObject = this;
  };
  ~NodeArtist() {
    if(artist->nodeObject == this) {
      artist->nodeObject = nullptr;
    }
  }
  static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getLink(Local<String> property, const AccessorInfo& info);
  static Handle<Value> browse(const Arguments& args);
  static Handle<Value> getTracks(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTophitTracks(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getAlbums(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getSimilarArtists(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getBiography(Local<String> property, const AccessorInfo& info);
  static void init();
};

#endif