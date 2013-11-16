/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#ifndef _NODE_PLAYER_H
#define _NODE_PLAYER_H

#include "NodeWrappedWithCallbacks.h"
#include <memory>
#include "../spotify/Player.h"

using namespace v8;

class NodePlayer : public NodeWrappedWithCallbacks<NodePlayer> {
private:
  std::shared_ptr<Player> player;
  NodePlayer(const NodePlayer& other) {};
public:
  NodePlayer();
  ~NodePlayer();
  static Handle<Value> stop(const Arguments& args);
  static Handle<Value> pause(const Arguments& args);
  static Handle<Value> resume(const Arguments& args);
  static Handle<Value> play(const Arguments& args);
  static Handle<Value> staticOn(const Arguments& args);
  static Handle<Value> getCurrentSecond(Local<String> property, const AccessorInfo& info);
  static Handle<Value> seek(const Arguments& args);
  static void init();
};

#endif