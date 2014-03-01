#include "NodePlaylistContainer.h"

NodePlaylistContainer::NodePlaylistContainer(std::shared_ptr<PlaylistContainer> _playlistContainer) : playlistContainer(_playlistContainer) {

}

NodePlaylistContainer::~NodePlaylistContainer() {

}

Handle<Value> NodePlaylistContainer::getOwner(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylistContainer::getPlaylists(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylistContainer::getStarred(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylistContainer::addPlaylist(const Arguments& args) {
  HandleScope scope;
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylistContainer::deletePlaylist(const Arguments& args) {
  HandleScope scope;
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylistContainer::movePlaylist(const Arguments& args) {
  HandleScope scope;
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylistContainer::isLoaded(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(Undefined());
}

void NodePlaylistContainer::init() {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("PlaylistContainer"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "off", off);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("owner"), getOwner);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("playlists"), getPlaylists);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("starred"), getStarred);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("isLoaded"), isLoaded);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "addPlaylist", addPlaylist);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "deletePlaylist", deletePlaylist);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "movePlaylist", movePlaylist);

  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}