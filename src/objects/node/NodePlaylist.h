#ifndef _NODE_PLAYLIST_H
#define _NODE_PLAYLIST_H

#include "NodeWrappedWithCallbacks.h"
#include "../spotify/Playlist.h"

#include <memory>

using namespace v8;

class NodePlaylist : public NodeWrappedWithCallbacks<NodePlaylist> {
private:
  std::shared_ptr<Playlist> playlist;
public:
  NodePlaylist(std::shared_ptr<Playlist> _playlist) : playlist(_playlist) {
    playlist->nodeObject = this;
  };
  ~NodePlaylist() {
    playlist->nodeObject = nullptr;
  }
  static void setName(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getLink(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getId(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTracks(const Arguments& args);

  static void init();
};

#endif