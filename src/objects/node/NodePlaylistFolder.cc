#include "NodePlaylistFolder.h"

NodePlaylistFolder::NodePlaylistFolder(std::shared_ptr<PlaylistFolder> _playlistFolder) : playlistFolder(_playlistFolder) {

};

NodePlaylistFolder::~NodePlaylistFolder() {

}

NAN_GETTER(NodePlaylistFolder::getName) {
  NodePlaylistFolder* nodePlaylistFolder = Nan::ObjectWrap::Unwrap<NodePlaylistFolder>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodePlaylistFolder->playlistFolder->name().c_str()).ToLocalChecked());
}

NAN_GETTER(NodePlaylistFolder::getType) {
  NodePlaylistFolder* nodePlaylistFolder = Nan::ObjectWrap::Unwrap<NodePlaylistFolder>(info.This());
  info.GetReturnValue().Set(Nan::New<Number>(nodePlaylistFolder->playlistFolder->type()));
}

void NodePlaylistFolder::init() {
  Local<FunctionTemplate> constructorTemplate = NodeWrapped::init("PlaylistFolder");
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("name").ToLocalChecked(), getName);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("type").ToLocalChecked(), getType);
  NodePlaylistFolder::constructorTemplate.Reset(constructorTemplate);
}
