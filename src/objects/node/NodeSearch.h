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

#ifndef _NODE_SEARCH_RESULT_H
#define _NODE_SEARCH_RESULT_H

#include "NodeWrapped.h"
#include "V8Browseable.h"
#include "../spotify/Search.h"

#include <v8.h>
#include <memory>

using namespace v8;

class NodeSearch : public NodeWrapped<NodeSearch>, public V8Browseable {
private:
  std::shared_ptr<Search> search;
  std::string searchQuery;
  int trackOffset, albumOffset, artistOffset, playlistOffset;
  int trackLimit, albumLimit, artistLimit, playlistLimit;
  void setupAdditionalMethods();
public:
  NodeSearch(const char* _query);
  NodeSearch(const char* _query, int offset);
  NodeSearch(const char* _query, int offset, int limit);

  static Handle<Value> getTrackOffset(Local<String> property, const AccessorInfo& info);
  static void setTrackOffset(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> getAlbumOffset(Local<String> property, const AccessorInfo& info);
  static void setAlbumOffset(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> getArtistOffset(Local<String> property, const AccessorInfo& info);
  static void setArtistOffset(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> getPlaylistOffset(Local<String> property, const AccessorInfo& info);
  static void setPlaylistOffset(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> getTrackLimit(Local<String> property, const AccessorInfo& info);
  static void setTrackLimit(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> getAlbumLimit(Local<String> property, const AccessorInfo& info);
  static void setAlbumLimit(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> getArtistLimit(Local<String> property, const AccessorInfo& info);
  static void setArtistLimit(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> getPlaylistLimit(Local<String> property, const AccessorInfo& info);
  static void setPlaylistLimit(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> didYouMean(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTotalTracks(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTotalAlbums(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTotalArtists(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTotalPlaylists(Local<String> property, const AccessorInfo& info);
  static Handle<Value> New(const Arguments& args);
  static Handle<Value> execute(const Arguments& args);
  static Handle<Value> getTracks(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getAlbums(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getArtists(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getPlaylists(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getLink(Local<String> property, const AccessorInfo& info);
  static void init();
};

#endif