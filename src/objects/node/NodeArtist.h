#ifndef _NODE_ARTIST_H
#define _NODE_ARTIST_H

#include "V8Browseable.h"
#include "../spotify/Artist.h"

#include <nan.h>
#include <memory>

using namespace v8;

class NodeArtist : public V8Browseable<NodeArtist> {
private:
  std::unique_ptr<Artist> artist;
public:
  NodeArtist(std::unique_ptr<Artist> artist);
  ~NodeArtist();
  static NAN_GETTER(getName);
  static NAN_GETTER(getLink);
  static NAN_METHOD(browse);
  static NAN_GETTER(getTracks);
  static NAN_GETTER(getTophitTracks);
  static NAN_GETTER(getAlbums);
  static NAN_GETTER(getSimilarArtists);
  static NAN_GETTER(getBiography);
  static NAN_GETTER(isLoaded);
  static void init();
};

#endif
