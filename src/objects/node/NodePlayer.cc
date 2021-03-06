#include "NodePlayer.h"
#include "NodeTrack.h"
#include "../../callbacks/SessionCallbacks.h"
#include "../../exceptions.h"

NodePlayer::NodePlayer(std::shared_ptr<Player> _player) : player(_player) {}

NodePlayer::~NodePlayer() {

}

NodePlayer::NodePlayer(const NodePlayer& other) {

}

NAN_METHOD(NodePlayer::pause) {
  NodePlayer* nodePlayer = Nan::ObjectWrap::Unwrap<NodePlayer>(info.This());
  nodePlayer->player->pause();
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlayer::stop) {
  NodePlayer* nodePlayer = Nan::ObjectWrap::Unwrap<NodePlayer>(info.This());
  nodePlayer->player->stop();
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlayer::resume) {
  NodePlayer* nodePlayer = Nan::ObjectWrap::Unwrap<NodePlayer>(info.This());
  nodePlayer->player->resume();
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlayer::play) {
  if(info.Length() < 1) {
    Nan::ThrowError("play needs a track as its first argument.");
    return;
  }
  NodePlayer* nodePlayer = Nan::ObjectWrap::Unwrap<NodePlayer>(info.This());
  NodeTrack* nodeTrack = Nan::ObjectWrap::Unwrap<NodeTrack>(info[0]->ToObject());
  try {
    nodePlayer->player->play(nodeTrack->track);
  } catch (const TrackNotPlayableException& e) {
    Nan::ThrowError("Track not playable");
    return;
  }
#ifndef NODE_SPOTIFY_NATIVE_SOUND
  catch (const NoAudioHandlerException& e) {
    Nan::ThrowError("No audio handler registered. Use spotify.useNodejsAudio().");
    return;
  }
#endif

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlayer::seek) {
  if(info.Length() < 1 || !info[0]->IsNumber()) {
    Nan::ThrowError("seek needs an integer as its first argument.");
    return;
  }
  NodePlayer* nodePlayer = Nan::ObjectWrap::Unwrap<NodePlayer>(info.This());
  int second = info[0]->ToInteger()->Value();
  nodePlayer->player->seek(second);
  info.GetReturnValue().SetUndefined();
}

NAN_GETTER(NodePlayer::getCurrentSecond) {
  NodePlayer* nodePlayer = Nan::ObjectWrap::Unwrap<NodePlayer>(info.This());
  info.GetReturnValue().Set(Nan::New<Integer>(nodePlayer->player->currentSecond));
}

NAN_METHOD(NodePlayer::on) {
  if(info.Length() < 1 || !info[0]->IsObject()) {
    Nan::ThrowError("on needs an object as its first argument.");
    return;
  }
  Handle<Object> callbacks = info[0]->ToObject();
  Handle<String> endOfTrackKey = Nan::New<String>("endOfTrack").ToLocalChecked();
  if(callbacks->Has(endOfTrackKey)) {
    SessionCallbacks::endOfTrackCallback.SetFunction(callbacks->Get(endOfTrackKey).As<Function>());
  }
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(NodePlayer::off) {
  //SessionCallbacks::endOfTrackCallback = Nan::Callback();
  info.GetReturnValue().SetUndefined();
}

void NodePlayer::init() {
  Local<FunctionTemplate> constructorTemplate = Nan::New<FunctionTemplate>();
  constructorTemplate->SetClassName(Nan::New<String>("Player").ToLocalChecked());
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  Nan::SetPrototypeMethod(constructorTemplate, "on", on);
  Nan::SetPrototypeMethod(constructorTemplate, "off", off);

  Nan::SetPrototypeMethod(constructorTemplate, "play", play);
  Nan::SetPrototypeMethod(constructorTemplate, "pause", pause);
  Nan::SetPrototypeMethod(constructorTemplate, "resume", resume);
  Nan::SetPrototypeMethod(constructorTemplate, "stop", stop);
  Nan::SetPrototypeMethod(constructorTemplate, "seek", seek);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("currentSecond").ToLocalChecked(), &getCurrentSecond);
  NodePlayer::constructorTemplate.Reset(constructorTemplate);
}
