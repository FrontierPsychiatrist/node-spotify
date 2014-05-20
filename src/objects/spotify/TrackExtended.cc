/**
The MIT License (MIT)

Copyright (c) <2014> <Moritz Schulze>

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

#include "TrackExtended.h"

#include <iostream>

TrackExtended::TrackExtended(sp_track* _track, sp_playlist* _playlist, int _position) : Track(_track), playlist(_playlist), position(_position) {
  sp_playlist_add_ref(playlist);
}

TrackExtended::~TrackExtended() {
  sp_playlist_release(playlist);
}

std::shared_ptr<User> TrackExtended::creator() {
  std::shared_ptr<User> creator;
  sp_user* spCreator = sp_playlist_track_creator(playlist, position);
  if(spCreator != nullptr) {
    creator = std::make_shared<User>(spCreator);
  }
  return creator;
}

bool TrackExtended::seen() {
  return sp_playlist_track_seen(playlist, position);
}

void TrackExtended::seen(bool seen) {
  sp_playlist_track_set_seen(playlist, position, seen);
}

std::string TrackExtended::message()  {
  std::string message;
  const char* spMessage = sp_playlist_track_message(playlist, position);
  if(spMessage != nullptr) {
    message = std::string(spMessage);
  }
  return message;
}

double TrackExtended::createTime() {
  return sp_playlist_track_create_time(playlist, position);
}
