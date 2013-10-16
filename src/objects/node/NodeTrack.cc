#include "NodeTrack.h"
#include "NodeArtist.h"
#include "NodeAlbum.h"

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
    NodeArtist* nodeArtist = new NodeArtist(nodeTrack->track->artists()[i]);
    jsArtists->Set(Number::New(i), nodeArtist->getV8Object() );
  }
  return scope.Close(jsArtists); 
}

Handle<Value> NodeTrack::getAlbum(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.Holder());
  NodeAlbum* nodeAlbum = new NodeAlbum(nodeTrack->track->album());
  return scope.Close(nodeAlbum->getV8Object());
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

void NodeTrack::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Track");

  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("link"), getLink, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("duration"), getDuration, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("artists"), getArtists, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("album"), getAlbum, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("starred"), getStarred, setStarred);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("popularity"), getPopularity, emptySetter);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}