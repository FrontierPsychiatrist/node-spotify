#ifndef _NODE_PLAYER_H
#define _NODE_PLAYER_H

#include "NodeWrapped.h"
#include "../spotify/Player.h"

#include <v8.h>
#include <memory>

using namespace v8;

class NodePlayer : public NodeWrapped<NodePlayer> {
private:
  std::shared_ptr<Player> player;
  NodePlayer(const NodePlayer& other);
public:
  NodePlayer(std::shared_ptr<Player> player);
  ~NodePlayer();
  static NAN_METHOD(stop);
  static NAN_METHOD(pause);
  static NAN_METHOD(resume);
  static NAN_METHOD(play);
  static NAN_GETTER(getCurrentSecond);
  static NAN_METHOD(seek);
  static NAN_METHOD(on);
  static NAN_METHOD(off);
  static void init();
};

#endif
