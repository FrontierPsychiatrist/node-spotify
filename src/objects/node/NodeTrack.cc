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

#include "NodeTrack.h"
#include "NodeArtist.h"
#include "NodeAlbum.h"

NodeTrack::NodeTrack(std::shared_ptr<Track> _track) : track(_track) {

};

NodeTrack::~NodeTrack() {

}

Handle<Value> NodeTrack::getName(Local<String> property, const AccessorInfo& info) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.Holder());
  return String::New(nodeTrack->track->name().c_str());
}

Handle<Value> NodeTrack::getLink(Local<String> property, const AccessorInfo& info) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.Holder());
  return String::New(nodeTrack->track->link().c_str());
}

Handle<Value> NodeTrack::getDuration(Local<String> property, const AccessorInfo& info) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.Holder());
  return Integer::New(nodeTrack->track->duration()/1000);
}

Handle<Value> NodeTrack::getPopularity(Local<String> property, const AccessorInfo& info) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.Holder());
  return Integer::New(nodeTrack->track->popularity());
}

Handle<Value> NodeTrack::getArtists(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.Holder());
  Local<Array> jsArtists = Array::New(nodeTrack->track->artists().size());
  for(int i = 0; i < (int)nodeTrack->track->artists().size(); i++) {
    if(nodeTrack->track->artists()[i].use_count() > 0) {
      NodeArtist* nodeArtist = new NodeArtist(nodeTrack->track->artists()[i]);
      jsArtists->Set(Number::New(i), nodeArtist->getV8Object() );
    } else {
      jsArtists->Set(Number::New(i), Undefined() );
    }
  }
  return scope.Close(jsArtists);
}

Handle<Value> NodeTrack::getAlbum(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.Holder());
  if(nodeTrack->track->album().use_count() > 0) {
    NodeAlbum* nodeAlbum = new NodeAlbum(nodeTrack->track->album());
    return scope.Close(nodeAlbum->getV8Object());
  } else {
    return scope.Close(Undefined());
  }
}

Handle<Value> NodeTrack::getStarred(Local<String> property, const AccessorInfo& info) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.Holder());
  return Boolean::New(nodeTrack->track->starred());
}

void NodeTrack::setStarred(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.Holder());
  nodeTrack->track->setStarred(value->ToBoolean()->Value());
  scope.Close(Undefined());
}

Handle<Value> NodeTrack::isLoaded(Local<String> property, const AccessorInfo& info) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.Holder());
  return Boolean::New(nodeTrack->track->isLoaded());
}

void NodeTrack::setMethods(Handle<FunctionTemplate> constructorTemplate) {
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("link"), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("duration"), getDuration);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("artists"), getArtists);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("album"), getAlbum);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("starred"), getStarred, setStarred);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("popularity"), getPopularity);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("isLoaded"), isLoaded);
}

void NodeTrack::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Track");
  setMethods(constructorTemplate);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}