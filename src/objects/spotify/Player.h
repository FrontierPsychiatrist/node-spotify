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

#ifndef _PLAYER_H
#define _PLAYER_H

#include <libspotify/api.h>
#include <memory>

#include "../node/V8Callable.h"
#include "Track.h"

class Player {
friend class NodePlayer;
friend class SessionCallbacks;
public:
  Player() {};
  void stop();
  void pause();
  void resume();
  void play(std::shared_ptr<Track> track);
  void seek(int second);
  void setCurrentSecond(int second);
private:
  int currentSecond;
  bool isPaused;
  Player(const Player& other) {};
  static std::shared_ptr<Player> instance;
  V8Callable* nodeObject;
};

#endif
