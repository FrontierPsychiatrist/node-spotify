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

#ifndef _PLAYLIST_CONTAINER_CALLBACKS_H
#define _PLAYLIST_CONTAINER_CALLBACKS_H

#include "../objects/node/V8Wrapped.h"

#include <initializer_list>
#include <libspotify/api.h>
#include <v8.h>

using namespace v8;

class PlaylistContainerCallbacksHolder {
private:
  sp_playlistcontainer* playlistContainer;
  sp_playlistcontainer_callbacks* playlistContainerCallbacks;
  V8Wrapped* userdata;
  void call(Handle<Function> callback, std::initializer_list<Handle<Value>> args);
public:
  PlaylistContainerCallbacksHolder(sp_playlistcontainer* pc, V8Wrapped* userdata);
  ~PlaylistContainerCallbacksHolder();

  //libspotify callback functions
  static void playlistAdded(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, void* userdata);
  static void playlistRemoved(sp_playlistcontainer *pc, sp_playlist *playlist, int position, void *userdata);
  static void playlistMoved(sp_playlistcontainer *pc, sp_playlist *playlist, int position, int new_position, void *userdata);

  Handle<Function> playlistAddedCallback;
  Handle<Function> playlistRemovedCallback;
  Handle<Function> playlistMovedCallback;

  void setCallbacks();
  void unsetCallbacks();
};

#endif