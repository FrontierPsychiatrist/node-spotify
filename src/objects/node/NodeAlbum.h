#ifndef _NODE_ALBUM_H
#define _NODE_ALBUM_H

#include "NodeWrapped.h"
#include "../spotify/Album.h"

#include <memory>

using namespace v8;

class NodeAlbum : public NodeWrapped<NodeAlbum> {
private:
  std::shared_ptr<Album> album;
public:
  NodeAlbum(std::shared_ptr<Album> _album) : album(_album) {};
  static void init();
  static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getLink(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getCoverBase64(const Arguments& args);
};

#endif