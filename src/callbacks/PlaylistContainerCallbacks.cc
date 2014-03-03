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

#include "PlaylistContainerCallbacks.h"
#include "../objects/node/NodePlaylist.h"
#include "../objects/spotify/Playlist.h"
#include "../events.h"
#include "../Application.h"

#include <memory>
#include <v8.h>

using namespace v8;

extern Application* application;

void PlaylistContainerCallbacks::playlistAdded(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, void* userdata) {
  auto nodeObjects = application->playlistContainerMapper->getObjects(pc);
  if(!nodeObjects.empty()) {
    auto playlist = std::make_shared<Playlist>(spPlaylist);
    NodePlaylist* nodePlaylist = new NodePlaylist(playlist);
    for(V8Callable* nodeObject : nodeObjects) {
      nodeObject->call(PLAYLIST_ADDED, {Undefined(), nodePlaylist->getV8Object(), Number::New(position)});
    }
  }
}

void PlaylistContainerCallbacks::playlistRemoved(sp_playlistcontainer *pc, sp_playlist *playlist, int position, void *userdata) {
auto nodeObjects = application->playlistContainerMapper->getObjects(pc);
  if(!nodeObjects.empty()) {
    for(V8Callable* nodeObject : nodeObjects) {
      nodeObject->call(PLAYLIST_REMOVED, {Undefined(), Number::New(position)});
    }
  }
}

void PlaylistContainerCallbacks::rootPlaylistContainerLoaded(sp_playlistcontainer* spPlaylistContainer, void* userdata) {

}