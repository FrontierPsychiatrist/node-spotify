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

void NodePlaylist::setName(Local<String> property, Local<Value> value, const AccessorInfo& info) {

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

Handle<Value> NodePlaylist::isLoaded(Local<String> property, const AccessorInfo& info) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.Holder());
  return Boolean::New(nodePlaylist->playlist->isLoaded());
}

Handle<Value> NodePlaylist::New(const Arguments& args) {
  HandleScope scope;
  NodePlaylist* nodePlaylist;
  String::Utf8Value playlistName(args[0]->ToString());
  try {
    std::shared_ptr<Playlist> playlist = application->playlistContainer->addPlaylist(std::string(*playlistName));
    nodePlaylist = new NodePlaylist(playlist);
  } catch(const PlaylistCreationException& e) {
    return scope.Close(V8_EXCEPTION("Playlist creation failed"));
  }
  nodePlaylist->Wrap(args.This());
  return scope.Close(args.This());
}

Handle<Function> NodePlaylist::getConstructor() {
  HandleScope scope;
  return scope.Close(constructor);
}

void NodePlaylist::init() {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New(New);
  constructorTemplate->SetClassName(String::NewSymbol("Playlist"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "off", off);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, setName);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("link"), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("isLoaded"), isLoaded);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getTracks", getTracks);

  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}