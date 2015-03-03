#ifndef _NODE_ALBUM_H
#define _NODE_ALBUM_H

#include "V8Browseable.h"
#include "../spotify/Album.h"

#include <nan.h>
#include <memory>

using namespace v8;

class NodeAlbum : public V8Browseable<NodeAlbum> {
friend class AlbumBrowseCallbacks;
private:
  std::unique_ptr<Album> album;
public:
  NodeAlbum(std::unique_ptr<Album> album);
  ~NodeAlbum();
  static void init();
  static NAN_GETTER(getName);
  static NAN_GETTER(getLink);
  static NAN_METHOD(getCoverBase64);
  static NAN_METHOD(browse);
  static NAN_GETTER(getTracks);
  static NAN_GETTER(getCopyrights);
  static NAN_GETTER(getReview);
  static NAN_GETTER(getArtist);
  static NAN_GETTER(isLoaded);
};

#endif
