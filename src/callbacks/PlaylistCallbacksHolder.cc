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

#include "PlaylistCallbacksHolder.h"
#include "../objects/spotify/Track.h"
#include "../objects/node/NodeTrack.h"
#include "../objects/spotify/Playlist.h"

#include <memory>

PlaylistCallbacksHolder::PlaylistCallbacksHolder(V8Wrapped* _userdata, sp_playlist* _playlist) : userdata(_userdata), playlist(_playlist) {
  playlistCallbacks = new sp_playlist_callbacks();
}

PlaylistCallbacksHolder::~PlaylistCallbacksHolder() {
  sp_playlist_remove_callbacks(playlist, playlistCallbacks, this);
  delete playlistCallbacks;
}

void PlaylistCallbacksHolder::call(Handle<Function> callback, std::initializer_list<Handle<Value>> args) {  
  unsigned int argc = args.size();
  Handle<Value>* argv = const_cast<Handle<Value>*>(args.begin());
  callback->Call(Context::GetCurrent()->Global(), argc, argv);
}

void PlaylistCallbacksHolder::playlistRenamed(sp_playlist* spPlaylist, void* userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  holder->call(holder->playlistRenamedCallback, { Undefined(), holder->userdata->getV8Object() });
}

void PlaylistCallbacksHolder::tracksAdded(sp_playlist* spPlaylist, sp_track *const *tracks, int num_tracks, int position, void *userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Handle<Array> nodeTracks = Array::New(num_tracks);
  for(int i = 0; i < num_tracks; i++) {
    NodeTrack* nodeTrack = new NodeTrack(std::make_shared<Track>(tracks[i]));
    nodeTracks->Set(Number::New(i), nodeTrack->getV8Object());
  }
  holder->call(holder->tracksAddedCallback, { Undefined(), holder->userdata->getV8Object(), nodeTracks, Number::New(position) });
}

void PlaylistCallbacksHolder::tracksMoved(sp_playlist* spPlaylist, const int* tracks, int num_tracks, int new_position, void *userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Handle<Array> movedTrackIndices = Array::New(num_tracks);
  for(int i = 0; i < num_tracks; i++) {
    movedTrackIndices->Set(Number::New(i), Number::New(tracks[i]));
  }
  holder->call(holder->tracksMovedCallback, { Undefined(), holder->userdata->getV8Object(), movedTrackIndices, Number::New(new_position) });
}

void PlaylistCallbacksHolder::tracksRemoved(sp_playlist* spPlaylist, const int *tracks, int num_tracks, void *userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Handle<Array> removedTrackIndexes = Array::New(num_tracks);
  for(int i = 0; i < num_tracks; i++) {
    removedTrackIndexes->Set(Number::New(i), Number::New(tracks[i]));
  }
  holder->call(holder->tracksRemovedCallback, { Undefined(), holder->userdata->getV8Object(), removedTrackIndexes });
}

void PlaylistCallbacksHolder::setCallbacks() {
  sp_playlist_remove_callbacks(playlist, playlistCallbacks, this);
  
  if(!playlistRenamedCallback.IsEmpty() && playlistRenamedCallback->IsCallable()) {
    playlistCallbacks->playlist_renamed = &PlaylistCallbacksHolder::playlistRenamed;  
  }
  if(!tracksAddedCallback.IsEmpty() && tracksAddedCallback->IsCallable()) {
    playlistCallbacks->tracks_added = &PlaylistCallbacksHolder::tracksAdded;  
  }
  if(!tracksMovedCallback.IsEmpty() && tracksMovedCallback->IsCallable()) {
    playlistCallbacks->tracks_moved = &PlaylistCallbacksHolder::tracksMoved;  
  }
  if(!tracksRemovedCallback.IsEmpty() && tracksRemovedCallback->IsCallable()) {
    playlistCallbacks->tracks_removed = &PlaylistCallbacksHolder::tracksRemoved;  
  }
  sp_playlist_add_callbacks(playlist, playlistCallbacks, this);
}

void PlaylistCallbacksHolder::unsetCallbacks() {
  sp_playlist_remove_callbacks(playlist, playlistCallbacks, this);
}