#include "Player.h"
#include "../SpotifyService/SpotifyService.h"
#include "../events.h"

extern "C" {
#include "../audio/audio.h"
}

#include <pthread.h>
#include <glog/logging.h>

extern SpotifyService* spotifyService;
extern audio_fifo_t g_audiofifo;

/* REMOVE ME */
namespace spotify {
extern int framesReceived;
extern int currentSecond;
}

void imageLoadedCallback(sp_image* image, void* userdata);

Handle<Value> Player::pause(const Arguments& args) {
  DLOG(INFO) << "Player pausing";
	return Player::simpleCall(args, &Player::spotifyPause);
}

Handle<Value> Player::stop(const Arguments& args) {
	return Player::simpleCall(args, &Player::spotifyStop);
}

Handle<Value> Player::resume(const Arguments& args) {
  DLOG(INFO) << "Player resuming";
	return Player::simpleCall(args, &Player::spotifyResume);
}

Handle<Value> Player::play(const Arguments& args) {
  spotify::framesReceived = 0;
  spotify::currentSecond = 0;
  Player* player = node::ObjectWrap::Unwrap<Player>(args.This());
  Track* track = node::ObjectWrap::Unwrap<Track>(args[0]->ToObject());
  player->track = track;
  return Player::simpleCall(args, &Player::spotifyPlay);
}

void Player::spotifyPause() {
  sp_session_player_play(spotifyService->spotifySession, 0);
  audio_fifo_flush(&g_audiofifo);
  isPaused = true;
}

void Player::spotifyResume() {
  if(isPaused) {
    sp_session_player_play(spotifyService->spotifySession, 1);
	  isPaused = false;
  }
}

void Player::spotifyStop() {
  sp_session_player_unload(spotifyService->spotifySession);
}

void Player::spotifyPlay() {
  sp_session_player_load(spotifyService->spotifySession, track->spotifyTrack);
  sp_session_player_play(spotifyService->spotifySession, 1);
}

void Player::setCurrentSecond(int _currentSecond) {
  currentSecond = _currentSecond;
  call(PLAYER_SECOND_IN_SONG);
}

//TODO: generic refactoring for NodeWrapped!
Handle<Value> Player::staticOn(const Arguments& args) {
  HandleScope scope;
  Player* player = node::ObjectWrap::Unwrap<Player>(args.This());
  String::Utf8Value callbackName(args[0]->ToString());
  Handle<Function> fun = Handle<Function>::Cast(args[1]);
  player->on( *callbackName, Persistent<Function>::New(fun));
  return scope.Close(Undefined());
}

Handle<Value> Player::getCurrentSecond(Local<String> property, const AccessorInfo& info) {
  Player* player = node::ObjectWrap::Unwrap<Player>(info.Holder());
  return Integer::New(player->currentSecond);
}

Handle<Value> Player::getCurrentlyPlayingData(const Arguments& args) {
  HandleScope scope;
  Player* player = node::ObjectWrap::Unwrap<Player>(args.This());
  pthread_mutex_lock(&player->lockingMutex);
  Handle<Object> data = Object::New();
  data->Set(String::New("track"), player->track->getV8Object());
  pthread_mutex_unlock(&player->lockingMutex);
  return scope.Close(data);
}

void Player::init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("Player"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
	
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "play", play);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "pause", pause);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "resume", resume);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "stop", stop);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", staticOn);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getCurrentlyPlayingData", getCurrentlyPlayingData);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("currentSecond"), &getCurrentSecond, emptySetter);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}
