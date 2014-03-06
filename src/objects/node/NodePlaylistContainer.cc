#include "NodePlaylistContainer.h"
#include "NodePlaylist.h"
#include "NodePlaylistFolder.h"
#include "../../exceptions.h"
#include "../../common_macros.h"
#include "../../Application.h"

extern Application* application;

NodePlaylistContainer::NodePlaylistContainer(std::shared_ptr<PlaylistContainer> _playlistContainer) : playlistContainer(_playlistContainer) {
  application->playlistContainerMapper->addObject(playlistContainer->playlistContainer, this);
}

NodePlaylistContainer::~NodePlaylistContainer() {
  application->playlistContainerMapper->removeObject(playlistContainer->playlistContainer, this);
}

Handle<Value> NodePlaylistContainer::getOwner(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylistContainer::getPlaylists(const Arguments& args) {
  HandleScope scope;
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
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

Handle<Value> NodePlaylistContainer::getStarred(const Arguments& args) {
  HandleScope scope;
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  NodePlaylist* starredPlaylist = new NodePlaylist(nodePlaylistContainer->playlistContainer->starredPlaylist());
  return scope.Close(starredPlaylist->getV8Object());
}

Handle<Value> NodePlaylistContainer::addPlaylist(const Arguments& args) {
  HandleScope scope;
  if(args.Length() < 1 || !args[0]->IsString()) {
    return scope.Close(V8_EXCEPTION("addPlaylist needs a string as its argument"));
  }
  String::Utf8Value playlistName(args[0]->ToString());
  try {
    application->playlistContainer->addPlaylist(std::string(*playlistName));
  } catch(const PlaylistCreationException& e) {
    return scope.Close(V8_EXCEPTION("Playlist creation failed"));
  }
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylistContainer::addFolder(const Arguments& args) {
  HandleScope scope;
  if(args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsString()) {
    return scope.Close(V8_EXCEPTION("addFolder needs a number and a string as arguments"));
  }
  int index = args[0]->ToNumber()->IntegerValue();
  String::Utf8Value folderName(args[1]->ToString());
  try {
    application->playlistContainer->addFolder(index, std::string(*folderName));
  } catch(const PlaylistCreationException& e) {
    return scope.Close(V8_EXCEPTION("Folder creation failed"));
  }
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylistContainer::deletePlaylist(const Arguments& args) {
  HandleScope scope;
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return scope.Close(V8_EXCEPTION("Please provide a number as the first argument"));
  }
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  int position = args[0]->ToNumber()->IntegerValue();
  nodePlaylistContainer->playlistContainer->removePlaylist(position);
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylistContainer::movePlaylist(const Arguments& args) {
  HandleScope scope;
  if(args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsNumber()) {
    return scope.Close(V8_EXCEPTION("Move playlist needs 2 numbers as its first arguments"));
  }
  int index = args[0]->ToNumber()->IntegerValue();
  int newPosition = args[1]->ToNumber()->IntegerValue();
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  try {
    nodePlaylistContainer->playlistContainer->movePlaylist(index, newPosition);
  } catch(const PlaylistNotMoveableException& e) {
    return scope.Close(V8_EXCEPTION(e.message.c_str()));
  }
  return scope.Close(Undefined());
}

Handle<Value> NodePlaylistContainer::isLoaded(Local<String> property, const AccessorInfo& info) {
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.Holder());
  return Boolean::New(nodePlaylistContainer->playlistContainer->isLoaded());
}

void NodePlaylistContainer::init() {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("PlaylistContainer"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "off", off);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("owner"), getOwner);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("isLoaded"), isLoaded);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getPlaylists", getPlaylists);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getStarred", getStarred);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "addPlaylist", addPlaylist);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "addFolder", addFolder);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "deletePlaylist", deletePlaylist);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "movePlaylist", movePlaylist);

  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}