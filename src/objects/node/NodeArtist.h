#ifndef _NODE_ARTIST_H
#define _NODE_ARTIST_H

#include <memory>
#include "NodeWrappedWithCallbacks.h"
#include "../spotify/Artist.h"

using namespace v8;

class NodeArtist : public NodeWrappedWithCallbacks<NodeArtist> {
private:
  std::shared_ptr<Artist> artist;
public:
  NodeArtist(std::shared_ptr<Artist> _artist) : artist(_artist) {};
  static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getLink(Local<String> property, const AccessorInfo& info);
  static Handle<Value> browse(const Arguments& args);
  static Handle<Value> getTracks(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getTophitTracks(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getAlbums(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getSimilarArtists(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getBiography(Local<String> property, const AccessorInfo& info);
  static void init();
};

#endif