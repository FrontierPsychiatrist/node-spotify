#include "NodeUser.h"
#include "NodePlaylistContainer.h"
#include "NodePlaylist.h"

NodeUser::NodeUser(std::unique_ptr<User> _user) : user(std::move(_user)) {}

NodeUser::~NodeUser() {}

NAN_GETTER(NodeUser::getLink) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodeUser->user->link().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeUser::getCanonicalName) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodeUser->user->canonicalName().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeUser::getDisplayName) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodeUser->user->displayName().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeUser::isLoaded) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  info.GetReturnValue().Set(Nan::New<Boolean>(nodeUser->user->isLoaded()));
}

NAN_GETTER(NodeUser::getPublishedPlaylistsContainer) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  auto playlistContainer = nodeUser->user->publishedPlaylists();
  NodePlaylistContainer* nodePlaylistContainer = new NodePlaylistContainer(playlistContainer);
  info.GetReturnValue().Set(nodePlaylistContainer->createInstance());
}

NAN_GETTER(NodeUser::getStarredPlaylist) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  auto playlist = nodeUser->user->starredPlaylist();
  NodePlaylist* nodePlaylist = new NodePlaylist(playlist);
  info.GetReturnValue().Set(nodePlaylist->createInstance());
}

void NodeUser::init() {
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("User");
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("canonicalName").ToLocalChecked(), getCanonicalName);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("link").ToLocalChecked(), getLink);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("displayName").ToLocalChecked(), getDisplayName);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("isLoaded").ToLocalChecked(), isLoaded);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("playlistContainer").ToLocalChecked(), getPublishedPlaylistsContainer);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("starredPlaylist").ToLocalChecked(), getStarredPlaylist);
  NodeUser::constructorTemplate.Reset(constructorTemplate);
}
