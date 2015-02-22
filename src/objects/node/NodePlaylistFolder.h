#ifndef _NODE_PLAYLIST_FOLDER_H
#define _NODE_PLAYLIST_FOLDER_H

#include "NodeWrapped.h"
#include "../spotify/PlaylistFolder.h"

#include <nan.h>
#include <memory>

using namespace v8;

class NodePlaylistFolder : public NodeWrapped<NodePlaylistFolder> {
private:
  std::shared_ptr<PlaylistFolder> playlistFolder;
public:
  NodePlaylistFolder(std::shared_ptr<PlaylistFolder> playlistFolder);
  ~NodePlaylistFolder();
  static void init();
  static NAN_GETTER(getName);
  static NAN_GETTER(getType);
};

#endif
