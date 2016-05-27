#include "NodeUser.h"
#include "NodePlaylistContainer.h"
#include "NodePlaylist.h"

NodeUser::NodeUser(std::unique_ptr<User> _user) : user(std::move(_user)) {}

NodeUser::~NodeUser() {}

NAN_GETTER(NodeUser::getLink) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  NanReturnValue(Nan::New<String>(nodeUser->user->link().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeUser::getCanonicalName) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  NanReturnValue(Nan::New<String>(nodeUser->user->canonicalName().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeUser::getDisplayName) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  NanReturnValue(Nan::New<String>(nodeUser->user->displayName().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeUser::isLoaded) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  NanReturnValue(NanNew<Boolean>(nodeUser->user->isLoaded()));
}

NAN_GETTER(NodeUser::getPublishedPlaylistsContainer) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  auto playlistContainer = nodeUser->user->publishedPlaylists();
  NodePlaylistContainer* nodePlaylistContainer = new NodePlaylistContainer(playlistContainer);
  NanReturnValue(nodePlaylistContainer->createInstance());
}

NAN_GETTER(NodeUser::getStarredPlaylist) {
  NodeUser* nodeUser = node::ObjectWrap::Unwrap<NodeUser>(info.This());
  auto playlist = nodeUser->user->starredPlaylist();
  NodePlaylist* nodePlaylist = new NodePlaylist(playlist);
  NanReturnValue(nodePlaylist->createInstance());
}

void NodeUser::init() {
  NanScope();
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("User");
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("canonicalName").ToLocalChecked(), getCanonicalName);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("link").ToLocalChecked(), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("displayName").ToLocalChecked(), getDisplayName);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("isLoaded").ToLocalChecked(), isLoaded);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("playlistContainer").ToLocalChecked(), getPublishedPlaylistsContainer);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("starredPlaylist").ToLocalChecked(), getStarredPlaylist);
  NanAssignPersistent(NodeUser::constructorTemplate, constructorTemplate);
}
