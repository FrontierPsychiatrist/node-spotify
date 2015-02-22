#include "NodePlaylistFolder.h"

NodePlaylistFolder::NodePlaylistFolder(std::shared_ptr<PlaylistFolder> _playlistFolder) : playlistFolder(_playlistFolder) {

};

NodePlaylistFolder::~NodePlaylistFolder() {

}

NAN_GETTER(NodePlaylistFolder::getName) {
  NanScope();
  NodePlaylistFolder* nodePlaylistFolder = node::ObjectWrap::Unwrap<NodePlaylistFolder>(args.This());
  NanReturnValue(NanNew<String>(nodePlaylistFolder->playlistFolder->name().c_str()));
}

NAN_GETTER(NodePlaylistFolder::getType) {
  NanScope();
  NodePlaylistFolder* nodePlaylistFolder = node::ObjectWrap::Unwrap<NodePlaylistFolder>(args.This());
  NanReturnValue(NanNew<Number>(nodePlaylistFolder->playlistFolder->type()));
}

void NodePlaylistFolder::init() {
  NanScope();
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("PlaylistFolder");
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("name"), getName);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("type"), getType);
  NanAssignPersistent(NodePlaylistFolder::constructorTemplate, constructorTemplate);
}