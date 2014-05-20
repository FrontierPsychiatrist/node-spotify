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

#ifndef _NODE_PLAYLIST_H
#define _NODE_PLAYLIST_H

#include "NodeWrapped.h"
#include "../../callbacks/PlaylistCallbacksHolder.h"
#include "../spotify/Playlist.h"

#include <v8.h>
#include <map>
#include <string>
#include <memory>

using namespace v8;

class NodePlaylist : public NodeWrapped<NodePlaylist> {
private:
  std::shared_ptr<Playlist> playlist;
  PlaylistCallbacksHolder playlistCallbacksHolder;
public:
  NodePlaylist(std::shared_ptr<Playlist> playlist);
  ~NodePlaylist();
  static void setName(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
  static void setCollaborative(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> getCollaborative(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getLink(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getDescription(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getNumTracks(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTrack(const Arguments& args);
  static Handle<Value> addTracks(const Arguments& args);
  static Handle<Value> removeTracks(const Arguments& args);
  static Handle<Value> reorderTracks(const Arguments& args);
  static Handle<Value> isLoaded(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getOwner(Local<String> property, const AccessorInfo& info);
  static Handle<Value> on(const Arguments& args);
  static Handle<Value> off(const Arguments& args);
  static void init();
};

#endif