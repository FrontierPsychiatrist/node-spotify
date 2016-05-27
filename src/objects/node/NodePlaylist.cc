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
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  Nan::Utf8String newName(value);
  nodePlaylist->playlist->name(*newName);
}

NAN_GETTER(NodePlaylist::getName) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodePlaylist->playlist->name().c_str()).ToLocalChecked());
}

NAN_SETTER(NodePlaylist::setCollaborative) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  nodePlaylist->playlist->setCollaborative(value->ToBoolean()->Value());
}

NAN_GETTER(NodePlaylist::getCollaborative) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<Boolean>(nodePlaylist->playlist->isCollaborative()));
}

NAN_GETTER(NodePlaylist::getLink) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodePlaylist->playlist->link().c_str()).ToLocalChecked());
}

NAN_GETTER(NodePlaylist::getDescription) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodePlaylist->playlist->description().c_str()).ToLocalChecked());
}

NAN_GETTER(NodePlaylist::getNumTracks) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<Integer>(nodePlaylist->playlist->numTracks()));
}

NAN_METHOD(NodePlaylist::getTrack) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  if(info.Length() < 1 || !info[0]->IsNumber()) {
    Nan::ThrowError("getTrack needs a number as its first argument.");
    return;
  }
  int position = info[0]->ToNumber()->IntegerValue();
  if(position >= nodePlaylist->playlist->numTracks() || position < 0) {
    Nan::ThrowError("Track index out of bounds");
    return;
  }
  std::shared_ptr<TrackExtended> track = nodePlaylist->playlist->getTrack(position);
  NodeTrackExtended* nodeTrack = new NodeTrackExtended(track);
  info.GetReturnValue().Set(nodeTrack->createInstance());
}

NAN_METHOD(NodePlaylist::addTracks) {
  if(info.Length() < 2 || !info[0]->IsArray() || !info[1]->IsNumber()) {
    Nan::ThrowError("addTracks needs an array and a number as its arguments.");
    return;
  }
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  Handle<Array> trackArray = Handle<Array>::Cast(info[0]);
  std::vector<std::shared_ptr<Track>> tracks(trackArray->Length());
  for(unsigned int i = 0; i < trackArray->Length(); i++) {
    Handle<Object> trackObject = trackArray->Get(i)->ToObject();
    NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(trackObject);
    tracks[i] = nodeTrack->track;
  }
  int position = info[1]->ToNumber()->IntegerValue();
  try {
    nodePlaylist->playlist->addTracks(tracks, position);
  } catch(const TracksNotAddedException& e) {
    Nan::ThrowError(e.message.c_str());
    return;
  }

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlaylist::removeTracks) {
  if(info.Length() < 1 || !info[0]->IsArray()) {
    Nan::ThrowError("removeTracks needs an array as its first argument.");
    return;
  }
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  Handle<Array> trackPositionsArray = Handle<Array>::Cast(info[0]);
  int trackPositions[trackPositionsArray->Length()];
  for(unsigned int i = 0; i < trackPositionsArray->Length(); i++) {
    trackPositions[i] = trackPositionsArray->Get(i)->ToNumber()->IntegerValue();
  }
  try {
    nodePlaylist->playlist->removeTracks(trackPositions, trackPositionsArray->Length());
  } catch(const TracksNotRemoveableException& e) {
    Nan::ThrowError("Tracks not removeable, permission denied.");
    return;
  }

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlaylist::reorderTracks) {
  if(info.Length() < 2 || !info[0]->IsArray() || !info[1]->IsNumber()) {
    Nan::ThrowError("reorderTracks needs an array and a numer as its arguments.");
    return;
  }
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  Handle<Array> trackPositionsArray = Handle<Array>::Cast(info[0]);
  int trackPositions[trackPositionsArray->Length()];
  int newPosition = info[1]->ToNumber()->IntegerValue();
  for(unsigned int i = 0; i < trackPositionsArray->Length(); i++) {
    trackPositions[i] = trackPositionsArray->Get(i)->ToNumber()->IntegerValue();
  }
  try {
    nodePlaylist->playlist->reorderTracks(trackPositions, trackPositionsArray->Length(), newPosition);
  } catch(const TracksNotReorderableException& e) {
    Nan::ThrowError(e.message.c_str());
    return;
  }

  info.GetReturnValue().SetUndefined();
}

NAN_GETTER(NodePlaylist::isLoaded) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<Boolean>(nodePlaylist->playlist->isLoaded()));
}

NAN_GETTER(NodePlaylist::getOwner) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  Handle<Value> owner;
  if(nodePlaylist->playlist->owner()) {
    owner = (new NodeUser(nodePlaylist->playlist->owner()))->createInstance();
  }
  info.GetReturnValue().Set(owner);
}

/**
  Set all callbacks for this playlist. Replaces all old callbacks.
**/
NAN_METHOD(NodePlaylist::on) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  if(info.Length() < 1 || !info[0]->IsObject()) {
    Nan::ThrowError("on needs an object as its first argument.");
    return;
  }
  Handle<Object> callbacks = info[0]->ToObject();
  Handle<String> playlistRenamedKey = Nan::New<String>("playlistRenamed").ToLocalChecked();
  Handle<String> tracksMovedKey = Nan::New<String>("tracksMoved").ToLocalChecked();
  Handle<String> tracksAddedKey = Nan::New<String>("tracksAdded").ToLocalChecked();
  Handle<String> tracksRemovedKey = Nan::New<String>("tracksRemoved").ToLocalChecked();
  Handle<String> trackCreatedChangedKey = Nan::New<String>("trackCreatedChanged").ToLocalChecked();
  Handle<String> trackSeenChangedKey = Nan::New<String>("trackSeenChanged").ToLocalChecked();
  Handle<String> trackMessageChangedKey = Nan::New<String>("trackMessageChanged").ToLocalChecked();
  nodePlaylist->playlistCallbacksHolder.playlistRenamedCallback.SetFunction(V8Utils::getFunctionFromObject(callbacks, playlistRenamedKey));
  nodePlaylist->playlistCallbacksHolder.tracksAddedCallback.SetFunction(V8Utils::getFunctionFromObject(callbacks, tracksAddedKey));
  nodePlaylist->playlistCallbacksHolder.tracksMovedCallback.SetFunction(V8Utils::getFunctionFromObject(callbacks, tracksMovedKey));
  nodePlaylist->playlistCallbacksHolder.tracksRemovedCallback.SetFunction(V8Utils::getFunctionFromObject(callbacks, tracksRemovedKey));
  nodePlaylist->playlistCallbacksHolder.trackCreatedChangedCallback.SetFunction(V8Utils::getFunctionFromObject(callbacks, trackCreatedChangedKey));
  nodePlaylist->playlistCallbacksHolder.trackSeenChangedCallback.SetFunction(V8Utils::getFunctionFromObject(callbacks, trackSeenChangedKey));
  nodePlaylist->playlistCallbacksHolder.trackMessageChangedCallback.SetFunction(V8Utils::getFunctionFromObject(callbacks, trackMessageChangedKey));
  nodePlaylist->playlistCallbacksHolder.setCallbacks();
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlaylist::off) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  nodePlaylist->playlistCallbacksHolder.unsetCallbacks();
  info.GetReturnValue().SetUndefined();
}

void NodePlaylist::init() {
  Local<FunctionTemplate> constructorTemplate = Nan::New<FunctionTemplate>();
  constructorTemplate->SetClassName(Nan::New<String>("Playlist").ToLocalChecked());
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  Nan::SetMethod(constructorTemplate, "on", on);
  Nan::SetMethod(constructorTemplate, "off", off);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("name").ToLocalChecked(), getName, setName);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("link").ToLocalChecked(), getLink);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("collaborative").ToLocalChecked(), getCollaborative, setCollaborative);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("description").ToLocalChecked(), getDescription);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("owner").ToLocalChecked(), getOwner);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("isLoaded").ToLocalChecked(), isLoaded);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("numTracks").ToLocalChecked(), getNumTracks);
  Nan::SetMethod(constructorTemplate, "getTrack", getTrack);
  Nan::SetMethod(constructorTemplate, "addTracks", addTracks);
  Nan::SetMethod(constructorTemplate, "removeTracks", removeTracks);
  Nan::SetMethod(constructorTemplate, "reorderTracks", reorderTracks);

  NodePlaylist::constructorTemplate.Reset(constructorTemplate);
}
