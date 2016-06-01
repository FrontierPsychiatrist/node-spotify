#include "PlaylistContainerCallbacksHolder.h"
#include "../objects/node/NodePlaylist.h"
#include "../objects/node/NodePlaylistFolder.h"
#include "../objects/spotify/Playlist.h"

using namespace v8;

PlaylistContainerCallbacksHolder::PlaylistContainerCallbacksHolder(sp_playlistcontainer* pc, Nan::ObjectWrap* _userdata) :
  playlistContainer(pc), userdata(_userdata) {
    playlistContainerCallbacks = new sp_playlistcontainer_callbacks();
  }

PlaylistContainerCallbacksHolder::~PlaylistContainerCallbacksHolder() {
  sp_playlistcontainer_remove_callbacks(playlistContainer, playlistContainerCallbacks, this);
  delete playlistContainerCallbacks;
}

void PlaylistContainerCallbacksHolder::playlistAdded(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, void* userdata) {
  Nan::HandleScope scope;
  auto holder = static_cast<PlaylistContainerCallbacksHolder*>(userdata);
  sp_playlist_type playlistType = sp_playlistcontainer_playlist_type(pc, position);
  std::shared_ptr<PlaylistBase> playlistBase;
  Local<Object> playlistV8;
  if(playlistType == SP_PLAYLIST_TYPE_PLAYLIST) {
    playlistBase = Playlist::fromCache(spPlaylist);
    NodeWrapped<NodePlaylist>* nodePlaylist = new NodePlaylist(Playlist::fromCache(spPlaylist));
    playlistV8 = nodePlaylist->createInstance();
  } else if(playlistType == SP_PLAYLIST_TYPE_START_FOLDER) {
    char buf[256];
    sp_playlistcontainer_playlist_folder_name(pc, position, buf, 256);
    NodeWrapped<NodePlaylistFolder>* nodePlaylist = new NodePlaylistFolder(std::make_shared<PlaylistFolder>(buf, playlistType));
    playlistV8 = nodePlaylist->createInstance();
  } else if(playlistType == SP_PLAYLIST_TYPE_END_FOLDER) {
    NodeWrapped<NodePlaylistFolder>* nodePlaylist = new NodePlaylistFolder(std::make_shared<PlaylistFolder>(playlistType));
    playlistV8 = nodePlaylist->createInstance();
  } else {
    return;
  }
  Local<Value> argv[] = {Nan::Undefined(), playlistV8, Nan::New<Number>(position)};
  holder->playlistAddedCallback.Call(3, argv);
}

void PlaylistContainerCallbacksHolder::playlistRemoved(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, void *userdata) {
  Nan::HandleScope scope;
  auto holder = static_cast<PlaylistContainerCallbacksHolder*>(userdata);
  Nan::ObjectWrap* nodePlaylist = nullptr; //FIXME what??
  if(nodePlaylist != nullptr) {
    Local<Value> argv[] = {Nan::Undefined(), Nan::New<Number>(position), nodePlaylist->handle()};
    holder->playlistRemovedCallback.Call(3, argv);
  } else {
    Local<Value> argv[] = {Nan::Undefined(), Nan::New<Number>(position)};
    holder->playlistRemovedCallback.Call(2, argv);
  }
}

void PlaylistContainerCallbacksHolder::playlistMoved(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, int new_position, void *userdata) {
  Nan::HandleScope scope;
  auto holder = static_cast<PlaylistContainerCallbacksHolder*>(userdata);
  Nan::ObjectWrap* nodePlaylist = nullptr; //FIXME what?
  if(nodePlaylist != nullptr) {
    Local<Value> argv[] = {Nan::Undefined(), Nan::New<Number>(position), Nan::New<Number>(new_position), nodePlaylist->handle()};
    holder->playlistMovedCallback.Call(4, argv);
  } else {
    Local<Value> argv[] = {Nan::Undefined(), Nan::New<Number>(position), Nan::New<Number>(new_position)};
    holder->playlistMovedCallback.Call(3, argv);
  }
}

void PlaylistContainerCallbacksHolder::setCallbacks() {
  sp_playlistcontainer_remove_callbacks(playlistContainer, playlistContainerCallbacks, this);
  if(!playlistAddedCallback.IsEmpty()) {
    playlistContainerCallbacks->playlist_added = &PlaylistContainerCallbacksHolder::playlistAdded;
  }
  if(!playlistRemovedCallback.IsEmpty()) {
    playlistContainerCallbacks->playlist_removed = &PlaylistContainerCallbacksHolder::playlistRemoved;
  }
  if(!playlistMovedCallback.IsEmpty()) {
    playlistContainerCallbacks->playlist_moved = &PlaylistContainerCallbacksHolder::playlistMoved;
  }
  sp_playlistcontainer_add_callbacks(playlistContainer, playlistContainerCallbacks, this);
}

void PlaylistContainerCallbacksHolder::unsetCallbacks() {
  sp_playlistcontainer_remove_callbacks(playlistContainer, playlistContainerCallbacks, this);
}
