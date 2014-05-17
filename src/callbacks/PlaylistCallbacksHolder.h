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

#ifndef _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_HOLDER_H
#define _SPOTIFY_SERVICE_PLAYLIST_CALLBACKS_HOLDER_H

#include "../objects/node/V8Wrapped.h"

#include <libspotify/api.h>
#include <v8.h>
#include <initializer_list>

using namespace v8;

class PlaylistCallbacksHolder {
private:
  V8Wrapped* userdata;
  sp_playlist* playlist;
  sp_playlist_callbacks* playlistCallbacks;
  void call(Handle<Function> callback, std::initializer_list<Handle<Value>> args);
public:
  PlaylistCallbacksHolder(V8Wrapped* userdata, sp_playlist* playlist);
  ~PlaylistCallbacksHolder();

  //libspotify callback functions.
  static void playlistRenamed(sp_playlist* spPlaylist, void* userdata);
  static void tracksAdded(sp_playlist* playlist, sp_track *const *tracks, int num_tracks, int position, void *userdata);
  static void tracksMoved(sp_playlist* playlist, const int* tracks, int num_tracks, int new_position, void *userdata);
  static void tracksRemoved(sp_playlist* spPlaylist, const int *tracks, int num_tracks, void *userdata);
  
  Handle<Function> playlistRenamedCallback;
  Handle<Function> tracksAddedCallback;
  Handle<Function> tracksMovedCallback;
  Handle<Function> tracksRemovedCallback;
  /**
    Register the callbacks with libspotify. Will first remove old registered callbacks.
  **/
  void setCallbacks();
  /**
    Unregister all callbacks.
  **/
  void unsetCallbacks();
};

#endif
