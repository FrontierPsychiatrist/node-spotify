#ifndef _NODE_PLAYLIST_CONTAINER_H
#define _NODE_PLAYLIST_CONTAINER_H

#include "NodeWrapped.h"
#include "../spotify/PlaylistContainer.h"
#include "../../callbacks/PlaylistContainerCallbacksHolder.h"

#include <nan.h>
#include <memory>

using namespace v8;

class NodePlaylistContainer : public NodeWrapped<NodePlaylistContainer> {
private:
  std::shared_ptr<PlaylistContainer> playlistContainer;
  PlaylistContainerCallbacksHolder playlistContainerCallbacksHolder;
public:
  NodePlaylistContainer(std::shared_ptr<PlaylistContainer> playlistContainer);
  ~NodePlaylistContainer();
  static NAN_GETTER(getOwner);
  static NAN_METHOD(getPlaylist);
  static NAN_GETTER(isLoaded);
  static NAN_GETTER(getNumPlaylists);
  static NAN_METHOD(addPlaylist);
  static NAN_METHOD(addFolder);
  static NAN_METHOD(deletePlaylist);
  static NAN_METHOD(movePlaylist);
  static NAN_METHOD(on);
  static NAN_METHOD(off);
  static void init();
};

#endif
