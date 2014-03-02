#include "NodePlaylistContainer.h"
#include "NodePlaylist.h"
#include "NodePlaylistFolder.h"

#include "../../exceptions.h"
#include "../../common_macros.h"

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
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.Holder());
  std::vector<std::shared_ptr<PlaylistBase>> playlists = nodePlaylistContainer->playlistContainer->getPlaylists();
  Local<Array> nPlaylists = Array::New(playlists.size());
  for(int i = 0; i < (int)playlists.size(); i++) {
    if(!playlists[i]->isFolder) {
      NodePlaylist* nodePlaylist = new NodePlaylist(std::static_pointer_cast<Playlist>(playlists[i]));
      nPlaylists->Set(Number::New(i), nodePlaylist->getV8Object());
    } else {
      NodePlaylistFolder* nodePlaylistFolder = new NodePlaylistFolder(std::static_pointer_cast<PlaylistFolder>(playlists[i]));
      nPlaylists->Set(Number::New(i), nodePlaylistFolder->getV8Object());
    }
  }
  return scope.Close(nPlaylists);
}

Handle<Value> NodePlaylistContainer::getStarred(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.Holder());
  NodePlaylist* starredPlaylist = new NodePlaylist(nodePlaylistContainer->playlistContainer->starredPlaylist());
  return scope.Close(starredPlaylist->getV8Object());
}

Handle<Value> NodePlaylistContainer::addPlaylist(const Arguments& args) {
  HandleScope scope;
  String::Utf8Value playlistName(args[0]->ToString());
  try {
    application->playlistContainer->addPlaylist(std::string(*playlistName));
  } catch(const PlaylistCreationException& e) {
    return scope.Close(V8_EXCEPTION("Playlist creation failed"));
  }
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