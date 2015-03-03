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
  NanScope();
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  NodeUser* nodeUser = new NodeUser(nodePlaylistContainer->playlistContainer->owner());
  NanReturnValue(nodeUser->createInstance());
}

NAN_GETTER(NodePlaylistContainer::getNumPlaylists) {
  NanScope();
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  NanReturnValue(NanNew<Integer>(nodePlaylistContainer->playlistContainer->numPlaylists()));
}

NAN_METHOD(NodePlaylistContainer::getPlaylist) {
  NanScope();
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return NanThrowError("getPlaylist needs an interger as its first argument.");
  }
  int index = args[0]->ToNumber()->IntegerValue();
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  if(index < 0 || index >= nodePlaylistContainer->playlistContainer->numPlaylists()) {
    return NanThrowError("Index out of range.");
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

  NanReturnValue(outNodePlaylist);
}

NAN_METHOD(NodePlaylistContainer::addPlaylist) {
  NanScope();
  if(args.Length() < 1 || !args[0]->IsString()) {
    return NanThrowError("addPlaylist needs a string as its argument");
  }
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  String::Utf8Value playlistName(args[0]->ToString());
  try {
    nodePlaylistContainer->playlistContainer->addPlaylist(std::string(*playlistName));
  } catch(const PlaylistCreationException& e) {
    return NanThrowError("Playlist creation failed");
  }
  NanReturnUndefined();
}

NAN_METHOD(NodePlaylistContainer::addFolder) {
  NanScope();
  if(args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsString()) {
    return NanThrowError("addFolder needs a number and a string as arguments.");
  }
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  int index = args[0]->ToNumber()->IntegerValue();
  String::Utf8Value folderName(args[1]->ToString());
  try {
    nodePlaylistContainer->playlistContainer->addFolder(index, std::string(*folderName));
  } catch(const PlaylistCreationException& e) {
    return NanThrowError("Folder creation failed");
  }
  NanReturnUndefined();
}

NAN_METHOD(NodePlaylistContainer::deletePlaylist) {
  NanScope();
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return NanThrowError("deletePlaylist needs an integer as its first argument.");
  }
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  int position = args[0]->ToNumber()->IntegerValue();
  nodePlaylistContainer->playlistContainer->removePlaylist(position);
  NanReturnUndefined();
}

NAN_METHOD(NodePlaylistContainer::movePlaylist) {
  NanScope();
  if(args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsNumber()) {
    return NanThrowError("Move playlist needs 2 numbers as its first arguments.");
  }
  int index = args[0]->ToNumber()->IntegerValue();
  int newPosition = args[1]->ToNumber()->IntegerValue();
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  try {
    nodePlaylistContainer->playlistContainer->movePlaylist(index, newPosition);
  } catch(const PlaylistNotMoveableException& e) {
    return NanThrowError(e.message.c_str());
  }
  NanReturnUndefined();
}

NAN_GETTER(NodePlaylistContainer::isLoaded) {
  NanScope();
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  NanReturnValue(NanNew<Boolean>(nodePlaylistContainer->playlistContainer->isLoaded()));
}

NAN_METHOD(NodePlaylistContainer::on) {
  NanScope();
  if(args.Length() < 1 || !args[0]->IsObject()) {
    return NanThrowError("on needs an object as its first argument.");
  }
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  Handle<Object> callbacks = args[0]->ToObject();
  Handle<String> playlistAddedKey = NanNew<String>("playlistAdded");
  Handle<String> playlistMovedKey = NanNew<String>("playlistMoved");
  Handle<String> playlistRemovedKey = NanNew<String>("playlistRemoved");
  nodePlaylistContainer->playlistContainerCallbacksHolder.playlistAddedCallback = V8Utils::getFunctionFromObject(callbacks, playlistAddedKey);
  nodePlaylistContainer->playlistContainerCallbacksHolder.playlistMovedCallback = V8Utils::getFunctionFromObject(callbacks, playlistMovedKey);
  nodePlaylistContainer->playlistContainerCallbacksHolder.playlistRemovedCallback = V8Utils::getFunctionFromObject(callbacks, playlistRemovedKey);
  nodePlaylistContainer->playlistContainerCallbacksHolder.setCallbacks();
  NanReturnUndefined();
}

NAN_METHOD(NodePlaylistContainer::off) {
  NanScope();
  NodePlaylistContainer* nodePlaylistContainer = node::ObjectWrap::Unwrap<NodePlaylistContainer>(args.This());
  nodePlaylistContainer->playlistContainerCallbacksHolder.unsetCallbacks();
  NanReturnUndefined();
}

void NodePlaylistContainer::init() {
  NanScope();
  Local<FunctionTemplate> constructorTemplate = NanNew<FunctionTemplate>();
  constructorTemplate->SetClassName(NanNew<String>("PlaylistContainer"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "off", off);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("owner"), getOwner);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("numPlaylists"), getNumPlaylists);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("isLoaded"), isLoaded);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getPlaylist", getPlaylist);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "addPlaylist", addPlaylist);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "addFolder", addFolder);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "deletePlaylist", deletePlaylist);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "movePlaylist", movePlaylist);

  NanAssignPersistent(NodePlaylistContainer::constructorTemplate, constructorTemplate);
}
