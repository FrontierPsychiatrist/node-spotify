/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#include "NodePlaylist.h"
#include "../../events.h"
#include "../../Application.h"
#include "../../exceptions.h"
#include "../../common_macros.h"
#include "../spotify/Track.h"
#include "NodeTrack.h"

extern Application* application;

std::map<std::string, Persistent<Function>> NodePlaylist::staticCallbacks;

NodePlaylist::NodePlaylist(std::shared_ptr<Playlist> _playlist) : playlist(_playlist) {
  application->playlistMapper->setObject(playlist->playlist, this);
}

NodePlaylist::~NodePlaylist() {
  application->playlistMapper->removeObject(playlist->playlist, this);
}

void NodePlaylist::setName(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.Holder());
  String::Utf8Value newName(value->ToString());
  nodePlaylist->playlist->name(*newName);
}

Handle<Value> NodePlaylist::getName(Local<String> property, const AccessorInfo& info) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.Holder());
  return String::New(nodePlaylist->playlist->name().c_str());
}

Handle<Value> NodePlaylist::getLink(Local<String> property, const AccessorInfo& info) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.Holder());
  return String::New(nodePlaylist->playlist->link().c_str());
}

Handle<Value> NodePlaylist::getTracks(const Arguments& args) {
  HandleScope scope;
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(args.This());
  std::vector<std::shared_ptr<Track>> tracks = nodePlaylist->playlist->getTracks();
  Local<Array> outArray = Array::New(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    outArray->Set(Number::New(i), nodeTrack->getV8Object());
  }
  return scope.Close(outArray);
}

Handle<Value> NodePlaylist::addTracks(const Arguments& args) {
  HandleScope scope;
  if(args.Length() < 2) {
    return scope.Close(V8_EXCEPTION("addTracks needs 2 arguments"));
  }
  if(!args[0]->IsArray()) {
    return scope.Close(V8_EXCEPTION("First argument must be an array"));
  }
  if(!args[1]->IsNumber()) {
    return scope.Close(V8_EXCEPTION("Second argument must be a number"));
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
  nodePlaylist->playlist->addTracks(tracks, position);
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylist::removeTracks(const Arguments& args) {
  HandleScope scope;
  if(args.Length() < 1 || !args[0]->IsArray()) {
    return scope.Close(V8_EXCEPTION("removeTracks needs an array as its first argument"));
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
    return scope.Close(V8_EXCEPTION("Tracks not removeable, permission denied."));
  }
  
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylist::reorderTracks(const Arguments& args) {
  HandleScope scope;
  if(args.Length() < 2 || !args[0]->IsArray() || !args[1]->IsNumber()) {
    return scope.Close(V8_EXCEPTION("reorderTracks needs an array and a numer as its arguments"));
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
    return scope.Close(V8_EXCEPTION(e.message.c_str()));
  }
  
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylist::isLoaded(Local<String> property, const AccessorInfo& info) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.Holder());
  return Boolean::New(nodePlaylist->playlist->isLoaded());
}

Handle<Function> NodePlaylist::getCallback(std::string name) {
  Handle<Function> callback = NodeWrappedWithCallbacks<NodePlaylist>::getCallback(name);
  if(callback.IsEmpty()) {
    //search static callbacks instead
    auto iterator = staticCallbacks.find(name);
    if( iterator != staticCallbacks.end() ) {
      callback = iterator->second;
    }
  }
  return callback;
}

void NodePlaylist::init() {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("Playlist"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "off", off);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, setName);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("link"), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("isLoaded"), isLoaded);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getTracks", getTracks);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "addTracks", addTracks);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "removeTracks", removeTracks);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "reorderTracks", reorderTracks);

  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}