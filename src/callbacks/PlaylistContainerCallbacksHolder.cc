#include "PlaylistContainerCallbacksHolder.h"
#include "../objects/node/NodePlaylist.h"
#include "../objects/node/NodePlaylistFolder.h"
#include "../objects/spotify/Playlist.h"

using namespace v8;

PlaylistContainerCallbacksHolder::PlaylistContainerCallbacksHolder(sp_playlistcontainer* pc, node::ObjectWrap* _userdata) : 
  playlistContainer(pc), userdata(_userdata) {
    playlistContainerCallbacks = new sp_playlistcontainer_callbacks();
  }

PlaylistContainerCallbacksHolder::~PlaylistContainerCallbacksHolder() {
  sp_playlistcontainer_remove_callbacks(playlistContainer, playlistContainerCallbacks, this);
  delete playlistContainerCallbacks;
}

void PlaylistContainerCallbacksHolder::call(std::unique_ptr<NanCallback>& callback, std::initializer_list<Handle<Value>> args) {
  unsigned int argc = args.size();
  Handle<Value>* argv = const_cast<Handle<Value>*>(args.begin());
  callback->Call(argc, argv);
}

void PlaylistContainerCallbacksHolder::playlistAdded(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, void* userdata) {
  auto holder = static_cast<PlaylistContainerCallbacksHolder*>(userdata);
  sp_playlist_type playlistType = sp_playlistcontainer_playlist_type(pc, position);
  std::shared_ptr<PlaylistBase> playlistBase;
  Handle<Object> playlistV8;
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
  holder->call(holder->playlistAddedCallback, {NanUndefined(), playlistV8, NanNew<Number>(position)});
}

void PlaylistContainerCallbacksHolder::playlistRemoved(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, void *userdata) {
  auto holder = static_cast<PlaylistContainerCallbacksHolder*>(userdata);
  node::ObjectWrap* nodePlaylist = nullptr; //FIXME what??
  if(nodePlaylist != nullptr) {
    holder->call(holder->playlistRemovedCallback, {NanUndefined(), NanNew<Number>(position), NanObjectWrapHandle(nodePlaylist)});
  } else {
    holder->call(holder->playlistRemovedCallback, {NanUndefined(), NanNew<Number>(position)});
  }
}

void PlaylistContainerCallbacksHolder::playlistMoved(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, int new_position, void *userdata) {
  auto holder = static_cast<PlaylistContainerCallbacksHolder*>(userdata);
  node::ObjectWrap* nodePlaylist = nullptr; //FIXME what?
  if(nodePlaylist != nullptr) {
    holder->call(holder->playlistMovedCallback, {NanUndefined(), NanNew<Number>(position), NanNew<Number>(new_position), NanObjectWrapHandle(nodePlaylist)});
  } else {
    holder->call(holder->playlistMovedCallback, {NanUndefined(), NanNew<Number>(position), NanNew<Number>(new_position)});
  }
}

void PlaylistContainerCallbacksHolder::setCallbacks() {
  sp_playlistcontainer_remove_callbacks(playlistContainer, playlistContainerCallbacks, this);
  if(playlistAddedCallback && !playlistAddedCallback->IsEmpty()) {
    playlistContainerCallbacks->playlist_added = &PlaylistContainerCallbacksHolder::playlistAdded;
  }
  if(playlistRemovedCallback && !playlistRemovedCallback->IsEmpty()) {
    playlistContainerCallbacks->playlist_removed = &PlaylistContainerCallbacksHolder::playlistRemoved;
  }
  if(playlistMovedCallback && !playlistMovedCallback->IsEmpty()) {
    playlistContainerCallbacks->playlist_moved = &PlaylistContainerCallbacksHolder::playlistMoved;
  }
  sp_playlistcontainer_add_callbacks(playlistContainer, playlistContainerCallbacks, this);
}

void PlaylistContainerCallbacksHolder::unsetCallbacks() {
  sp_playlistcontainer_remove_callbacks(playlistContainer, playlistContainerCallbacks, this);
}