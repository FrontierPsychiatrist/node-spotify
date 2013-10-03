#include "NodePlayer.h"
#include "NodeTrack.h"

#include "../../events.h"
#include "../../Application.h"
#include "../../SpotifyService/SpotifyService.h"

extern "C" {
  #include "../../audio/audio.h"
}

extern Application* application;

/* REMOVE ME */
namespace spotify {
extern int framesReceived;
extern int currentSecond;
}

Handle<Value> NodePlayer::pause(const Arguments& args) {
  HandleScope scope;
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  auto cb = [=] () { 
    sp_session_player_play(application->session, 0);
    audio_fifo_flush(&application->audio_fifo);
    nodePlayer->isPaused = true;
  };
  application->spotifyService->executeSpotifyAPIcall(cb);
  return scope.Close(Undefined());
}

Handle<Value> NodePlayer::stop(const Arguments& args) {
  HandleScope scope;
  auto cb = [=] () { 
    sp_session_player_unload(application->session);
  };
  application->spotifyService->executeSpotifyAPIcall(cb);
  return scope.Close(Undefined());
}

Handle<Value> NodePlayer::resume(const Arguments& args) {
  HandleScope scope;
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
    auto cb = [=] () { 
    if(nodePlayer->isPaused) {
      sp_session_player_play(application->session, 1);
      nodePlayer->isPaused = false;
    }
  };
  application->spotifyService->executeSpotifyAPIcall(cb);
  return scope.Close(Undefined());
}

Handle<Value> NodePlayer::play(const Arguments& args) {
  HandleScope scope;
  spotify::framesReceived = 0;
  spotify::currentSecond = 0;
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(args.This());
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(args[0]->ToObject());
  auto cb = [=] () {
    sp_session_player_load(application->session, nodeTrack->track->track);
    sp_session_player_play(application->session, 1);
  };
  application->spotifyService->executeSpotifyAPIcall(cb);
  return scope.Close(Undefined());
}

void NodePlayer::setCurrentSecond(int _currentSecond) {
  currentSecond = _currentSecond;
  call(PLAYER_SECOND_IN_SONG);
}

Handle<Value> NodePlayer::getCurrentSecond(Local<String> property, const AccessorInfo& info) {
  NodePlayer* nodePlayer = node::ObjectWrap::Unwrap<NodePlayer>(info.Holder());
  return Integer::New(nodePlayer->currentSecond);
}

void NodePlayer::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Player");
    
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "play", play);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "pause", pause);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "resume", resume);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "stop", stop);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("currentSecond"), &getCurrentSecond, emptySetter);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}
