#include "NodePlaylist.h"
#include "../../events.h"
#include "../spotify/Track.h"
#include "NodeTrack.h"

void NodePlaylist::setName(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.Holder());
  String::Utf8Value v8Str(value);
  nodePlaylist->playlist->name = *v8Str;
}

Handle<Value> NodePlaylist::getName(Local<String> property, const AccessorInfo& info) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.Holder());
  return String::New(nodePlaylist->playlist->name.c_str());
}

Handle<Value> NodePlaylist::getLink(Local<String> property, const AccessorInfo& info) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.Holder());
  return String::New(nodePlaylist->playlist->link.c_str());
}

Handle<Value> NodePlaylist::getId(Local<String> property, const AccessorInfo& info) {
  NodePlaylist* nodePlaylist = node::ObjectWrap::Unwrap<NodePlaylist>(info.Holder());
  return Integer::New(nodePlaylist->playlist->id);
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

void NodePlaylist::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrappedWithCallbacks::init("Playlist");

  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, setName);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("link"), getLink, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("id"), getId, emptySetter);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getTracks", getTracks);

  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}