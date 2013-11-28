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

#ifndef _NODE_TRACK_H
#define _NODE_TRACK_H

#include <memory>

#include "NodeWrapped.h"
#include "../spotify/Track.h"

using namespace v8;

class NodeTrack : public NodeWrapped<NodeTrack> {
friend class NodePlayer;
private:
  std::shared_ptr<Track> track;
public:
  NodeTrack(std::shared_ptr<Track> _track) : track(_track) {};
  static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getLink(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getArtists(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getDuration(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getAlbum(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getStarred(Local<String> property, const AccessorInfo& info);
  static Handle<Value> getPopularity(Local<String> property, const AccessorInfo& info);
  static Handle<Value> isLoaded(Local<String> property, const AccessorInfo& info);
  static void setStarred(Local<String> property, Local<Value> value, const AccessorInfo& info);
  static void init();
};

#endif