#include "NodePlaylistFolder.h"

NodePlaylistFolder::NodePlaylistFolder(std::shared_ptr<PlaylistFolder> _playlistFolder) : playlistFolder(_playlistFolder) {

};

NodePlaylistFolder::~NodePlaylistFolder() {

}

NAN_GETTER(NodePlaylistFolder::getName) {
  NodePlaylistFolder* nodePlaylistFolder = node::ObjectWrap::Unwrap<NodePlaylistFolder>(info.This());
  NanReturnValue(Nan::New<String>(nodePlaylistFolder->playlistFolder->name().c_str()).ToLocalChecked());
}

NAN_GETTER(NodePlaylistFolder::getType) {
  NodePlaylistFolder* nodePlaylistFolder = node::ObjectWrap::Unwrap<NodePlaylistFolder>(info.This());
  NanReturnValue(NanNew<Number>(nodePlaylistFolder->playlistFolder->type()));
}

void NodePlaylistFolder::init() {
  NanScope();
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("PlaylistFolder");
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("name").ToLocalChecked(), getName);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("type").ToLocalChecked(), getType);
  NanAssignPersistent(NodePlaylistFolder::constructorTemplate, constructorTemplate);
}