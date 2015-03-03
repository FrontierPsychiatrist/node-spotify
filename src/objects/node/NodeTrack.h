#ifndef _NODE_TRACK_H
#define _NODE_TRACK_H

#include "NodeWrapped.h"
#include "../spotify/Track.h"

#include <nan.h>
#include <memory>

using namespace v8;

class NodeTrack : public NodeWrapped<NodeTrack> {
friend class NodePlayer;
friend class NodePlaylist;
private:
  std::shared_ptr<Track> track;
public:
  NodeTrack(std::shared_ptr<Track> track);
  ~NodeTrack();
  static NAN_GETTER(getName);
  static NAN_GETTER(getLink);
  static NAN_GETTER(getArtists);
  static NAN_GETTER(getDuration);
  static NAN_GETTER(getAlbum);
  static NAN_GETTER(getStarred);
  static NAN_GETTER(getPopularity);
  static NAN_GETTER(isLoaded);
  static NAN_GETTER(getAvailability);
  static NAN_SETTER(setStarred);
  static Handle<FunctionTemplate> init();
};

#endif