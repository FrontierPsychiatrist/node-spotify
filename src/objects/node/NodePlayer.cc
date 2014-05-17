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

#include "NodePlayer.h"
#include "NodeTrack.h"
#include "../../callbacks/SessionCallbacks.h"
#include "../../exceptions.h"
#include "../../common_macros.h"

NodePlayer::NodePlayer() {
  player = player->instance;
}

NodePlayer::~NodePlayer() {

}

NodePlayer::NodePlayer(const NodePlayer& other) {

}

Handle<Value> NodePlayer::pause(const Arguments& args) {
  HandleScope scope;
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  nodePlayer->player->pause();
  return scope.Close(Undefined());
}

Handle<Value> NodePlayer::stop(const Arguments& args) {
  HandleScope scope;
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  nodePlayer->player->stop();
  return scope.Close(Undefined());
}

Handle<Value> NodePlayer::resume(const Arguments& args) {
  HandleScope scope;
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  nodePlayer->player->resume();
  return scope.Close(Undefined());
}

Handle<Value> NodePlayer::play(const Arguments& args) {
  HandleScope scope;
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args[0]->ToObject());
  try {
    nodePlayer->player->play(nodeTrack->track);
  } catch (const TrackNotPlayableException& e) {
    return scope.Close(V8_EXCEPTION("Track not playable"));
  }
  return scope.Close(Undefined());
}

Handle<Value> NodePlayer::seek(const Arguments& args) {
  HandleScope scope;
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  int second = args[0]->ToInteger()->Value();
  nodePlayer->player->seek(second);
  return scope.Close(Undefined());
}

Handle<Value> NodePlayer::getCurrentSecond(Local<String> property, const AccessorInfo& info) {
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(info.Holder());
  return Integer::New(nodePlayer->player->currentSecond);
}

Handle<Value> NodePlayer::on(const Arguments& args) {
  HandleScope scope;
  if(args.Length() < 1 || !args[0]->IsObject()) {
    return scope.Close(V8_EXCEPTION("on needs an object as its first argument."));
  }
  Handle<Object> callbacks = args[0]->ToObject();
  Handle<String> endOfTrackKey = String::New("endOfTrack");
  if(callbacks->Has(endOfTrackKey)) {
    SessionCallbacks::endOfTrackCallback = Persistent<Function>::New(Handle<Function>::Cast(callbacks->Get(endOfTrackKey)));
  }
  return scope.Close(Undefined());
}

Handle<Value> NodePlayer::off(const Arguments& args) {
  HandleScope scope;
  SessionCallbacks::endOfTrackCallback = Handle<Function>();
  return scope.Close(Undefined());
}

void NodePlayer::init() {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("Player"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "off", off);

  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "play", play);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "pause", pause);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "resume", resume);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "stop", stop);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "seek", seek);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("currentSecond"), &getCurrentSecond, emptySetter);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}
