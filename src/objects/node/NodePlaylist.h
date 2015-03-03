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
  static NAN_SETTER(setName);
  static NAN_GETTER(getName);
  static NAN_SETTER(setCollaborative);
  static NAN_GETTER(getCollaborative);
  static NAN_GETTER(getLink);
  static NAN_GETTER(getDescription);
  static NAN_GETTER(getNumTracks);
  static NAN_METHOD(getTrack);
  static NAN_METHOD(addTracks);
  static NAN_METHOD(removeTracks);
  static NAN_METHOD(reorderTracks);
  static NAN_GETTER(isLoaded);
  static NAN_GETTER(getOwner);
  static NAN_METHOD(on);
  static NAN_METHOD(off);
  static void init();
};

#endif
