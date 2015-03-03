#ifndef _NODE_USER_H
#define _NODE_USER_H

#include "NodeWrapped.h"
#include "../spotify/User.h"

#include <nan.h>
#include <memory>

using namespace v8;

class NodeUser : public NodeWrapped<NodeUser> {
private:
  std::unique_ptr<User> user;
public:
  NodeUser(std::unique_ptr<User> user);
  ~NodeUser();
  static NAN_GETTER(getCanonicalName);
  static NAN_GETTER(getDisplayName);
  static NAN_GETTER(isLoaded);
  static NAN_GETTER(getLink);
  static NAN_GETTER(getPublishedPlaylistsContainer);
  static NAN_GETTER(getStarredPlaylist);
  static void init();
};

#endif
