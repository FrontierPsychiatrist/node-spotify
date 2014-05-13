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

#include "PlaylistContainerCallbacksHolder.h"
#include "../objects/node/NodePlaylist.h"
#include "../objects/node/NodePlaylistFolder.h"
#include "../objects/spotify/Playlist.h"

#include <memory>

PlaylistContainerCallbacksHolder::PlaylistContainerCallbacksHolder(sp_playlistcontainer* pc, V8Wrapped* _userdata) : 
  playlistContainer(pc), userdata(_userdata) {
    playlistContainerCallbacks = new sp_playlistcontainer_callbacks();
  }

PlaylistContainerCallbacksHolder::~PlaylistContainerCallbacksHolder() {
  sp_playlistcontainer_remove_callbacks(playlistContainer, playlistContainerCallbacks, this);
  delete playlistContainerCallbacks;
}

void PlaylistContainerCallbacksHolder::call(Handle<Function> callback, std::initializer_list<Handle<Value>> args) {  
  unsigned int argc = args.size();
  Handle<Value>* argv = const_cast<Handle<Value>*>(args.begin());
  callback->Call(Context::GetCurrent()->Global(), argc, argv);
}

void PlaylistContainerCallbacksHolder::playlistAdded(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, void* userdata) {
  auto holder = static_cast<PlaylistContainerCallbacksHolder*>(userdata);
  sp_playlist_type playlistType = sp_playlistcontainer_playlist_type(pc, position);
  std::shared_ptr<PlaylistBase> playlistBase;
  V8Wrapped* nodePlaylist;
  if(playlistType == SP_PLAYLIST_TYPE_PLAYLIST) {
    playlistBase = Playlist::fromCache(spPlaylist);
    nodePlaylist = new NodePlaylist(Playlist::fromCache(spPlaylist));
  } else if(playlistType == SP_PLAYLIST_TYPE_START_FOLDER) {
    char buf[256];
    sp_playlistcontainer_playlist_folder_name(pc, position, buf, 256);
    nodePlaylist = new NodePlaylistFolder(std::make_shared<PlaylistFolder>(buf, playlistType));
  } else if(playlistType == SP_PLAYLIST_TYPE_END_FOLDER) {
    nodePlaylist = new NodePlaylistFolder(std::make_shared<PlaylistFolder>(playlistType));
  } else {
    return;
  }
  holder->call(holder->playlistAddedCallback, {Undefined(), nodePlaylist->getV8Object(), Number::New(position)});
}

void PlaylistContainerCallbacksHolder::playlistRemoved(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, void *userdata) {
  auto holder = static_cast<PlaylistContainerCallbacksHolder*>(userdata);
  V8Wrapped* nodePlaylist = nullptr;
  if(nodePlaylist != nullptr) {
    holder->call(holder->playlistRemovedCallback, {Undefined(), Number::New(position), nodePlaylist->getV8Object()});
  } else {
    holder->call(holder->playlistRemovedCallback, {Undefined(), Number::New(position)});
  }
}

void PlaylistContainerCallbacksHolder::playlistMoved(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, int new_position, void *userdata) {
  auto holder = static_cast<PlaylistContainerCallbacksHolder*>(userdata);
  V8Wrapped* nodePlaylist = nullptr;
  if(nodePlaylist != nullptr) {
    holder->call(holder->playlistMovedCallback, {Undefined(), Number::New(position), Number::New(new_position), nodePlaylist->getV8Object()});
  } else {
    holder->call(holder->playlistMovedCallback, {Undefined(), Number::New(position), Number::New(new_position)});
  }
}

void PlaylistContainerCallbacksHolder::setCallbacks() {
  sp_playlistcontainer_remove_callbacks(playlistContainer, playlistContainerCallbacks, this);
  if(!playlistAddedCallback.IsEmpty() && playlistAddedCallback->IsCallable()) {
    playlistContainerCallbacks->playlist_added = &PlaylistContainerCallbacksHolder::playlistAdded;
  }
  if(!playlistRemovedCallback.IsEmpty() && playlistRemovedCallback->IsCallable()) {
    playlistContainerCallbacks->playlist_removed = &PlaylistContainerCallbacksHolder::playlistRemoved;
  }
  if(!playlistMovedCallback.IsEmpty() && playlistMovedCallback->IsCallable()) {
    playlistContainerCallbacks->playlist_moved = &PlaylistContainerCallbacksHolder::playlistMoved;
  }
  sp_playlistcontainer_add_callbacks(playlistContainer, playlistContainerCallbacks, this);
}

void PlaylistContainerCallbacksHolder::unsetCallbacks() {
  sp_playlistcontainer_remove_callbacks(playlistContainer, playlistContainerCallbacks, this);
}