#include "NodeArtist.h"

#include "../../events.h"

Handle<Value> NodeArtist::getName(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());
  return scope.Close(String::New(nodeArtist->artist->name().c_str()));
}

Handle<Value> NodeArtist::getLink(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());
  return scope.Close(String::New(nodeArtist->artist->link().c_str()));
}

Handle<Value> NodeArtist::browse(const Arguments& args) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  if(nodeArtist->artist->artistBrowse == nullptr) {
    Persistent<Function> callback = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
    nodeArtist->on(ALBUMBROWSE_COMPLETE, callback);

    //Mutate the V8 object.
    Handle<Object> nodeArtistV8 = nodeArtist->getV8Object();
    nodeArtistV8->SetAccessor(String::NewSymbol("tracks"), getTracks);
    nodeArtistV8->SetAccessor(String::NewSymbol("tophitTracks"), getTophitTracks);
    nodeArtistV8->SetAccessor(String::NewSymbol("albums"), getAlbums);
    nodeArtistV8->SetAccessor(String::NewSymbol("similarArtists"), getSimilarArtists);
    nodeArtistV8->SetAccessor(String::NewSymbol("biography"), getBiography);
    //TODO: portraits

    nodeArtist->artist->browse();
  } else {
    nodeArtist->call(ARTISTBROWSE_COMPLETE);
  }
  return scope.Close(Undefined());
}

Handle<Value> NodeArtist::getTracks(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());

  return scope.Close(Undefined());
}

Handle<Value> NodeArtist::getTophitTracks(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());

  return scope.Close(Undefined());
}

Handle<Value> NodeArtist::getAlbums(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());

  return scope.Close(Undefined());
}

Handle<Value> NodeArtist::getSimilarArtists(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());

  return scope.Close(Undefined());
}

Handle<Value> NodeArtist::getBiography(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());

  return scope.Close(Undefined());
}

void NodeArtist::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Artist");
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("link"), getLink, emptySetter);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "browse", browse);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}