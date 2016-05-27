#include "NodePlaylistContainer.h"
#include "NodePlaylist.h"
#include "NodePlaylistFolder.h"
#include "NodeUser.h"
#include "../../exceptions.h"
#include "../../utils/V8Utils.h"

NodePlaylistContainer::NodePlaylistContainer(std::shared_ptr<PlaylistContainer> _playlistContainer) : playlistContainer(_playlistContainer),
  playlistContainerCallbacksHolder(playlistContainer->playlistContainer, this) {
}

NodePlaylistContainer::~NodePlaylistContainer() {
}

NAN_GETTER(NodePlaylistContainer::getOwner) {
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.This());
  NodeUser* nodeUser = new NodeUser(nodePlaylistContainer->playlistContainer->owner());
  info.GetReturnValue().Set(nodeUser->createInstance());
}

NAN_GETTER(NodePlaylistContainer::getNumPlaylists) {
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.This());
  info.GetReturnValue().Set(Nan::New<Integer>(nodePlaylistContainer->playlistContainer->numPlaylists()));
}

NAN_METHOD(NodePlaylistContainer::getPlaylist) {
  if(info.Length() < 1 || !info[0]->IsNumber()) {
    Nan::ThrowError("getPlaylist needs an interger as its first argument.");
    return;
  }
  int index = info[0]->ToNumber()->IntegerValue();
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.This());
  if(index < 0 || index >= nodePlaylistContainer->playlistContainer->numPlaylists()) {
    Nan::ThrowError("Index out of range.");
    return;
  }
  std::shared_ptr<PlaylistBase> playlist = nodePlaylistContainer->playlistContainer->getPlaylist(index);

  Handle<Value> outNodePlaylist;
  if(!playlist->isFolder) {
    NodePlaylist* nodePlaylist = new NodePlaylist(std::static_pointer_cast<Playlist>(playlist));
    outNodePlaylist = nodePlaylist->createInstance();
  } else {
    NodePlaylistFolder* nodePlaylistFolder = new NodePlaylistFolder(std::static_pointer_cast<PlaylistFolder>(playlist));
    outNodePlaylist = nodePlaylistFolder->createInstance();
  }

  info.GetReturnValue().Set(outNodePlaylist);
}

NAN_METHOD(NodePlaylistContainer::addPlaylist) {
  if(info.Length() < 1 || !info[0]->IsString()) {
    Nan::ThrowError("addPlaylist needs a string as its argument");
    return;
  }
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.This());
  String::Utf8Value playlistName(info[0]->ToString());
  try {
    nodePlaylistContainer->playlistContainer->addPlaylist(std::string(*playlistName));
  } catch(const PlaylistCreationException& e) {
    Nan::ThrowError("Playlist creation failed");
    return;
  }
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlaylistContainer::addFolder) {
  if(info.Length() < 2 || !info[0]->IsNumber() || !info[1]->IsString()) {
    Nan::ThrowError("addFolder needs a number and a string as arguments.");
    return;
  }
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.This());
  int index = info[0]->ToNumber()->IntegerValue();
  String::Utf8Value folderName(info[1]->ToString());
  try {
    nodePlaylistContainer->playlistContainer->addFolder(index, std::string(*folderName));
  } catch(const PlaylistCreationException& e) {
    Nan::ThrowError("Folder creation failed");
    return;
  }
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlaylistContainer::deletePlaylist) {
  if(info.Length() < 1 || !info[0]->IsNumber()) {
    Nan::ThrowError("deletePlaylist needs an integer as its first argument.");
    return;
  }
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.This());
  int position = info[0]->ToNumber()->IntegerValue();
  nodePlaylistContainer->playlistContainer->removePlaylist(position);
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlaylistContainer::movePlaylist) {
  if(info.Length() < 2 || !info[0]->IsNumber() || !info[1]->IsNumber()) {
    Nan::ThrowError("Move playlist needs 2 numbers as its first arguments.");
    return;
  }
  int index = info[0]->ToNumber()->IntegerValue();
  int newPosition = info[1]->ToNumber()->IntegerValue();
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.This());
  try {
    nodePlaylistContainer->playlistContainer->movePlaylist(index, newPosition);
  } catch(const PlaylistNotMoveableException& e) {
    Nan::ThrowError(e.message.c_str());
    return;
  }
  info.GetReturnValue().SetUndefined();
}

NAN_GETTER(NodePlaylistContainer::isLoaded) {
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.This());
  info.GetReturnValue().Set(Nan::New<Boolean>(nodePlaylistContainer->playlistContainer->isLoaded()));
}

NAN_METHOD(NodePlaylistContainer::on) {
  if(info.Length() < 1 || !info[0]->IsObject()) {
    Nan::ThrowError("on needs an object as its first argument.");
    return;
  }
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.This());
  Handle<Object> callbacks = info[0]->ToObject();
  Handle<String> playlistAddedKey = Nan::New<String>("playlistAdded").ToLocalChecked();
  Handle<String> playlistMovedKey = Nan::New<String>("playlistMoved").ToLocalChecked();
  Handle<String> playlistRemovedKey = Nan::New<String>("playlistRemoved").ToLocalChecked();
  nodePlaylistContainer->playlistContainerCallbacksHolder.playlistAddedCallback.SetFunction(V8Utils::getFunctionFromObject(callbacks, playlistAddedKey));
  nodePlaylistContainer->playlistContainerCallbacksHolder.playlistMovedCallback.SetFunction(V8Utils::getFunctionFromObject(callbacks, playlistMovedKey));
  nodePlaylistContainer->playlistContainerCallbacksHolder.playlistRemovedCallback.SetFunction(V8Utils::getFunctionFromObject(callbacks, playlistRemovedKey));
  nodePlaylistContainer->playlistContainerCallbacksHolder.setCallbacks();
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlaylistContainer::off) {
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(info.This());
  nodePlaylistContainer->playlistContainerCallbacksHolder.unsetCallbacks();
  info.GetReturnValue().SetUndefined();
}

void NodePlaylistContainer::init() {
  Local<FunctionTemplate> constructorTemplate = Nan::New<FunctionTemplate>();
  constructorTemplate->SetClassName(Nan::New<String>("PlaylistContainer").ToLocalChecked());
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  Nan::SetMethod(constructorTemplate, "on", on);
  Nan::SetMethod(constructorTemplate, "off", off);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("owner").ToLocalChecked(), getOwner);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("numPlaylists").ToLocalChecked(), getNumPlaylists);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("isLoaded").ToLocalChecked(), isLoaded);
  Nan::SetMethod(constructorTemplate, "getPlaylist", getPlaylist);
  Nan::SetMethod(constructorTemplate, "addPlaylist", addPlaylist);
  Nan::SetMethod(constructorTemplate, "addFolder", addFolder);
  Nan::SetMethod(constructorTemplate, "deletePlaylist", deletePlaylist);
  Nan::SetMethod(constructorTemplate, "movePlaylist", movePlaylist);

  NodePlaylistContainer::constructorTemplate.Reset(constructorTemplate);
}
