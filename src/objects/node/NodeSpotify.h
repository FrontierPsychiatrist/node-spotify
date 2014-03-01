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

#ifndef _NODE_SPOTIFY_H
#define _NODE_SPOTIFY_H

#include "NodeWrapped.h"
#include "../spotify/Spotify.h"
#include <v8.h>
#include <memory>

using namespace v8;

class NodeSpotify : public NodeWrapped<NodeSpotify> {
public:
  NodeSpotify(Handle<Object> option);
  ~NodeSpotify() {};
  static Handle<Value> login(const Arguments& args);
  static Handle<Value> logout(const Arguments& args);
  static Handle<Value> ready(const Arguments& args);
  static Handle<Value> getPlaylists(const Arguments& args);
  static Handle<Value> getPlaylistContainer(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getStarred(const Arguments& args);
  static Handle<Value> addPlaylist(const Arguments& args);
  static Handle<Value> getRememberedUser(Local<String> property, const AccessorInfo& info);
  static Handle<Value> createFromLink(const Arguments& args);
  static void init();
private:
  std::unique_ptr<Spotify> spotify;
};

#endif
