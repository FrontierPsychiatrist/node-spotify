#include "NodePlaylist.h"
#include "../../exceptions.h"
#include "../spotify/Track.h"
#include "../spotify/TrackExtended.h"
#include "NodeTrack.h"
#include "NodeTrackExtended.h"
#include "NodeUser.h"

NodePlaylist::NodePlaylist(std::shared_ptr<Playlist> _playlist) : playlist(_playlist),
  playlistCallbacksHolder(this, _playlist->playlist) {
}

NodePlaylist::~NodePlaylist() {
  
}

NAN_SETTER(NodePlaylist::setName) {
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  Nan::Utf8String newName(value);
  nodePlaylist->playlist->name(*newName);
}

NAN_GETTER(NodePlaylist::getName) {
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodePlaylist->playlist->name().c_str()).ToLocalChecked());
}

NAN_SETTER(NodePlaylist::setCollaborative) {
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  nodePlaylist->playlist->setCollaborative(value->ToBoolean()->Value());
}

NAN_GETTER(NodePlaylist::getCollaborative) {
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<Boolean>(nodePlaylist->playlist->isCollaborative()));
}

NAN_GETTER(NodePlaylist::getLink) {
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodePlaylist->playlist->link().c_str()).ToLocalChecked());
}

NAN_GETTER(NodePlaylist::getDescription) {
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodePlaylist->playlist->description().c_str()).ToLocalChecked());
}

NAN_GETTER(NodePlaylist::getNumTracks) {
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<Integer>(nodePlaylist->playlist->numTracks()));
}

NAN_METHOD(NodePlaylist::getTrack) {
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
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
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  Handle<Array> trackArray = Handle<Array>::Cast(info[0]);
  std::vector<std::shared_ptr<Track>> tracks(trackArray->Length());
  for(unsigned int i = 0; i < trackArray->Length(); i++) {
    Local<Object> trackObject = trackArray->Get(i)->ToObject();
    NodeTrack* nodeTrack = Nan::ObjectWrap::Unwrap<NodeTrack>(trackObject);
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
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
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
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
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
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  info.GetReturnValue().Set(Nan::New<Boolean>(nodePlaylist->playlist->isLoaded()));
}

NAN_GETTER(NodePlaylist::getOwner) {
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  Local<Value> owner;
  if(nodePlaylist->playlist->owner()) {
    owner = (new NodeUser(nodePlaylist->playlist->owner()))->createInstance();
  }
  info.GetReturnValue().Set(owner);
}

/**
  Set all callbacks for this playlist. Replaces all old callbacks.
**/
NAN_METHOD(NodePlaylist::on) {
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  if(info.Length() < 1 || !info[0]->IsObject()) {
    Nan::ThrowError("on needs an object as its first argument.");
    return;
  }
  Handle<Object> callbacks = info[0]->ToObject();
  Handle<String> playlistRenamedKey = Nan::New<String>("playlistRenamed").ToLocalChecked();
  if(callbacks->Has(playlistRenamedKey)) {
    nodePlaylist->playlistCallbacksHolder.playlistRenamedCallback.SetFunction(callbacks->Get(playlistRenamedKey).As<Function>());
  }

  Handle<String> tracksMovedKey = Nan::New<String>("tracksMoved").ToLocalChecked();
  if(callbacks->Has(tracksMovedKey)) {
    nodePlaylist->playlistCallbacksHolder.tracksAddedCallback.SetFunction(callbacks->Get(tracksMovedKey).As<Function>());
  }

  Handle<String> tracksAddedKey = Nan::New<String>("tracksAdded").ToLocalChecked();
  if(callbacks->Has(tracksAddedKey)) {
    nodePlaylist->playlistCallbacksHolder.tracksMovedCallback.SetFunction(callbacks->Get(tracksAddedKey).As<Function>());
  }

  Handle<String> tracksRemovedKey = Nan::New<String>("tracksRemoved").ToLocalChecked();
  if(callbacks->Has(tracksRemovedKey)) {
    nodePlaylist->playlistCallbacksHolder.tracksRemovedCallback.SetFunction(callbacks->Get(tracksRemovedKey).As<Function>());
  }

  Handle<String> trackCreatedChangedKey = Nan::New<String>("trackCreatedChanged").ToLocalChecked();
  if(callbacks->Has(trackCreatedChangedKey)) {
    nodePlaylist->playlistCallbacksHolder.trackCreatedChangedCallback.SetFunction(callbacks->Get(trackCreatedChangedKey).As<Function>());
  }

  Handle<String> trackSeenChangedKey = Nan::New<String>("trackSeenChanged").ToLocalChecked();
  if(callbacks->Has(trackSeenChangedKey)) {
    nodePlaylist->playlistCallbacksHolder.trackSeenChangedCallback.SetFunction(callbacks->Get(trackSeenChangedKey).As<Function>());
  }

  Handle<String> trackMessageChangedKey = Nan::New<String>("trackMessageChanged").ToLocalChecked();
  if(callbacks->Has(trackMessageChangedKey)) {
    nodePlaylist->playlistCallbacksHolder.trackMessageChangedCallback.SetFunction(callbacks->Get(trackMessageChangedKey).As<Function>());
  }
  nodePlaylist->playlistCallbacksHolder.setCallbacks();
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlaylist::off) {
  NodePlaylist* nodePlaylist = Nan::ObjectWrap::Unwrap<NodePlaylist>(info.This());
  nodePlaylist->playlistCallbacksHolder.unsetCallbacks();
  info.GetReturnValue().SetUndefined();
}

void NodePlaylist::init() {
  Local<FunctionTemplate> constructorTemplate = Nan::New<FunctionTemplate>();
  constructorTemplate->SetClassName(Nan::New<String>("Playlist").ToLocalChecked());
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  Nan::SetPrototypeMethod(constructorTemplate, "on", on);
  Nan::SetPrototypeMethod(constructorTemplate, "off", off);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("name").ToLocalChecked(), getName, setName);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("link").ToLocalChecked(), getLink);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("collaborative").ToLocalChecked(), getCollaborative, setCollaborative);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("description").ToLocalChecked(), getDescription);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("owner").ToLocalChecked(), getOwner);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("isLoaded").ToLocalChecked(), isLoaded);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("numTracks").ToLocalChecked(), getNumTracks);
  Nan::SetPrototypeMethod(constructorTemplate, "getTrack", getTrack);
  Nan::SetPrototypeMethod(constructorTemplate, "addTracks", addTracks);
  Nan::SetPrototypeMethod(constructorTemplate, "removeTracks", removeTracks);
  Nan::SetPrototypeMethod(constructorTemplate, "reorderTracks", reorderTracks);

  NodePlaylist::constructorTemplate.Reset(constructorTemplate);
}
