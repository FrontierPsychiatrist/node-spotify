#include "NodePlaylist.h"
#include "../../exceptions.h"
#include "../spotify/Track.h"
#include "../spotify/TrackExtended.h"
#include "NodeTrack.h"
#include "NodeTrackExtended.h"
#include "NodeUser.h"
#include "../../utils/V8Utils.h"

NodePlaylist::NodePlaylist(std::shared_ptr<Playlist> _playlist) : playlist(_playlist),
  playlistCallbacksHolder(this, _playlist->playlist) {
}

NodePlaylist::~NodePlaylist() {
  
}

NAN_SETTER(NodePlaylist::setName) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  NanUtf8String newName(value);
  nodePlaylist->playlist->name(*newName);
}

NAN_GETTER(NodePlaylist::getName) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  NanReturnValue(NanNew<String>(nodePlaylist->playlist->name().c_str()));
}

NAN_SETTER(NodePlaylist::setCollaborative) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  nodePlaylist->playlist->setCollaborative(value->ToBoolean()->Value());
}

NAN_GETTER(NodePlaylist::getCollaborative) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  NanReturnValue(NanNew<Boolean>(nodePlaylist->playlist->isCollaborative()));
}

NAN_GETTER(NodePlaylist::getLink) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  NanReturnValue(NanNew<String>(nodePlaylist->playlist->link().c_str()));
}

NAN_GETTER(NodePlaylist::getDescription) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  NanReturnValue(NanNew<String>(nodePlaylist->playlist->description().c_str()));
}

NAN_GETTER(NodePlaylist::getNumTracks) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  NanReturnValue(NanNew<Integer>(nodePlaylist->playlist->numTracks()));
}

NAN_METHOD(NodePlaylist::getTrack) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return NanThrowError("getTrack needs a number as its first argument.");
  }
  int position = args[0]->ToNumber()->IntegerValue();
  if(position >= nodePlaylist->playlist->numTracks() || position < 0) {
    return NanThrowError("Track index out of bounds");
  }
  std::shared_ptr<TrackExtended> track = nodePlaylist->playlist->getTrack(position);
  NodeTrackExtended* nodeTrack = new NodeTrackExtended(track);
  NanReturnValue(nodeTrack->createInstance());
}

NAN_METHOD(NodePlaylist::addTracks) {
  NanScope();
  if(args.Length() < 2 || !args[0]->IsArray() || !args[1]->IsNumber()) {
    return NanThrowError("addTracks needs an array and a number as its arguments.");
  }
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  Handle<Array> trackArray = Handle<Array>::Cast(args[0]);
  std::vector<std::shared_ptr<Track>> tracks(trackArray->Length());
  for(unsigned int i = 0; i < trackArray->Length(); i++) {
    Handle<Object> trackObject = trackArray->Get(i)->ToObject();
    NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(trackObject);
    tracks[i] = nodeTrack->track;
  }
  int position = args[1]->ToNumber()->IntegerValue();
  try {
    nodePlaylist->playlist->addTracks(tracks, position);
  } catch(const TracksNotAddedException& e) {
    return NanThrowError(e.message.c_str());
  }

  NanReturnUndefined();
}

NAN_METHOD(NodePlaylist::removeTracks) {
  NanScope();
  if(args.Length() < 1 || !args[0]->IsArray()) {
    return NanThrowError("removeTracks needs an array as its first argument.");
  }
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  Handle<Array> trackPositionsArray = Handle<Array>::Cast(args[0]);
  int trackPositions[trackPositionsArray->Length()];
  for(unsigned int i = 0; i < trackPositionsArray->Length(); i++) {
    trackPositions[i] = trackPositionsArray->Get(i)->ToNumber()->IntegerValue();
  }
  try {
    nodePlaylist->playlist->removeTracks(trackPositions, trackPositionsArray->Length());
  } catch(const TracksNotRemoveableException& e) {
    return NanThrowError("Tracks not removeable, permission denied.");
  }

  NanReturnUndefined();
}

NAN_METHOD(NodePlaylist::reorderTracks) {
  NanScope();
  if(args.Length() < 2 || !args[0]->IsArray() || !args[1]->IsNumber()) {
    return NanThrowError("reorderTracks needs an array and a numer as its arguments.");
  }
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  Handle<Array> trackPositionsArray = Handle<Array>::Cast(args[0]);
  int trackPositions[trackPositionsArray->Length()];
  int newPosition = args[1]->ToNumber()->IntegerValue();
  for(unsigned int i = 0; i < trackPositionsArray->Length(); i++) {
    trackPositions[i] = trackPositionsArray->Get(i)->ToNumber()->IntegerValue();
  }
  try {
    nodePlaylist->playlist->reorderTracks(trackPositions, trackPositionsArray->Length(), newPosition);
  } catch(const TracksNotReorderableException& e) {
    return NanThrowError(e.message.c_str());
  }

  NanReturnUndefined();
}

NAN_GETTER(NodePlaylist::isLoaded) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  NanReturnValue(NanNew<Boolean>(nodePlaylist->playlist->isLoaded()));
}

NAN_GETTER(NodePlaylist::getOwner) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  Handle<Value> owner;
  if(nodePlaylist->playlist->owner()) {
    owner = (new NodeUser(nodePlaylist->playlist->owner()))->createInstance();
  }
  NanReturnValue(owner);
}

/**
  Set all callbacks for this playlist. Replaces all old callbacks.
**/
NAN_METHOD(NodePlaylist::on) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  if(args.Length() < 1 || !args[0]->IsObject()) {
    return NanThrowError("on needs an object as its first argument.");
  }
  Handle<Object> callbacks = args[0]->ToObject();
  Handle<String> playlistRenamedKey = NanNew<String>("playlistRenamed");
  Handle<String> tracksMovedKey = NanNew<String>("tracksMoved");
  Handle<String> tracksAddedKey = NanNew<String>("tracksAdded");
  Handle<String> tracksRemovedKey = NanNew<String>("tracksRemoved");
  Handle<String> trackCreatedChangedKey = NanNew<String>("trackCreatedChanged");
  Handle<String> trackSeenChangedKey = NanNew<String>("trackSeenChanged");
  Handle<String> trackMessageChangedKey = NanNew<String>("trackMessageChanged");
  nodePlaylist->playlistCallbacksHolder.playlistRenamedCallback = V8Utils::getFunctionFromObject(callbacks, playlistRenamedKey);
  nodePlaylist->playlistCallbacksHolder.tracksAddedCallback = V8Utils::getFunctionFromObject(callbacks, tracksAddedKey);
  nodePlaylist->playlistCallbacksHolder.tracksMovedCallback = V8Utils::getFunctionFromObject(callbacks, tracksMovedKey);
  nodePlaylist->playlistCallbacksHolder.tracksRemovedCallback = V8Utils::getFunctionFromObject(callbacks, tracksRemovedKey);
  nodePlaylist->playlistCallbacksHolder.trackCreatedChangedCallback = V8Utils::getFunctionFromObject(callbacks, trackCreatedChangedKey);
  nodePlaylist->playlistCallbacksHolder.trackSeenChangedCallback = V8Utils::getFunctionFromObject(callbacks, trackSeenChangedKey);
  nodePlaylist->playlistCallbacksHolder.trackMessageChangedCallback = V8Utils::getFunctionFromObject(callbacks, trackMessageChangedKey);
  nodePlaylist->playlistCallbacksHolder.setCallbacks();
  NanReturnUndefined();
}

NAN_METHOD(NodePlaylist::off) {
  NanScope();
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  nodePlaylist->playlistCallbacksHolder.unsetCallbacks();
  NanReturnUndefined();
}

void NodePlaylist::init() {
  NanScope();
  Local<FunctionTemplate> constructorTemplate = NanNew<FunctionTemplate>();
  constructorTemplate->SetClassName(NanNew<String>("Playlist"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "off", off);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("name"), getName, setName);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("collaborative"), getCollaborative, setCollaborative);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("link"), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("description"), getDescription);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("isLoaded"), isLoaded);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("owner"), getOwner);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("numTracks"), getNumTracks);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getTrack", getTrack);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "addTracks", addTracks);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "removeTracks", removeTracks);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "reorderTracks", reorderTracks);

  NanAssignPersistent(NodePlaylist::constructorTemplate, constructorTemplate);
}
