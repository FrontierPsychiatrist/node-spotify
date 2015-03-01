#ifndef _NODE_TRACK_EXTENDED_H
#define _NODE_TRACK_EXTENDED_H

#include "NodeTrack.h"
#include "../spotify/TrackExtended.h"

#include <nan.h>
#include <memory>

using namespace v8;

class NodeTrackExtended : public NodeTrack {
private:
  std::shared_ptr<TrackExtended> trackExtended;
public:
  NodeTrackExtended(std::shared_ptr<TrackExtended> trackExtended);
  static NAN_GETTER(getCreator);
  static NAN_GETTER(getSeen);
  static NAN_SETTER(setSeen);
  static NAN_GETTER(getCreateTime);
  static NAN_GETTER(getMessage);
  static void init();
  static Handle<Function> getConstructor();
  Handle<Object> createInstance();
protected:
  static Persistent<FunctionTemplate> constructorTemplate;
};

#endif