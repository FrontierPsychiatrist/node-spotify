#ifndef _NODE_ALBUM_H
#define _NODE_ALBUM_H

#include "NodeWrappedWithCallbacks.h"
#include "../spotify/Album.h"

#include <memory>

using namespace v8;

class NodeAlbum : public NodeWrappedWithCallbacks<NodeAlbum> {
friend class AlbumBrowseCallbacks;
private:
  std::shared_ptr<Album> album;
public:
  NodeAlbum(std::shared_ptr<Album> _album) : album(_album) {
    album->nodeObject = this;
  };
  ~NodeAlbum() {
    if(album->nodeObject == this) {
      album->nodeObject = nullptr;
    }
  }
  static void init();
  static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getLink(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getCoverBase64(const Arguments& args);
  static Handle<Value> browse(const Arguments& args);
  static Handle<Value> getTracks(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getCopyrights(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getReview(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getArtist(Local<String> property, const AccessorInfo& info);
};

#endif