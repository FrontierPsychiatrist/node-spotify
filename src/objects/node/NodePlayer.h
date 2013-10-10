#ifndef _NODE_PLAYER_H
#define _NODE_PLAYER_H

#include "NodeWrapped.h"
#include <memory>

using namespace v8;

class NodePlayer : public NodeWrapped<NodePlayer> {
private:
  int currentSecond;
  bool isPaused;
  static std::unique_ptr<NodePlayer> instance;
  NodePlayer();
  NodePlayer(const NodePlayer& other);
public:
  static Handle<Value> stop(const Arguments& args);
  static Handle<Value> pause(const Arguments& args);
  static Handle<Value> resume(const Arguments& args);
  static Handle<Value> play(const Arguments& args);
  static Handle<Value> staticOn(const Arguments& args);
  static Handle<Value> getCurrentSecond(Local<String> property, const AccessorInfo& info);
  static Handle<Value> seek(const Arguments& args);

  void setCurrentSecond(int currentSecond);
  /**
  *   Callback track finished playing:
  *       - remove track from queue
  *       - play next track
  *       - update currentTrackPosition if necessary
  **/

  static void init();
  static NodePlayer& getInstance();
};

#endif