#include "Player.h"
#include "../SpotifyService/SpotifyService.h"
#include "../events.h"

extern "C" {
#include "../audio/audio.h"
}

#include <pthread.h>

extern SpotifyService* spotifyService;
extern audio_fifo_t g_audiofifo;

/* REMOVE ME */
namespace spotify {
extern int framesReceived;
extern int currentSecond;
}

void imageLoadedCallback(sp_image* image, void* userdata);

Handle<Value> Player::pause(const Arguments& args) {
  //DLOG(INFO) << "Player pausing";
  Player* player = node::ObjectWrap::Unwrap<Player>(args.This());
  auto cb = [=] () { 
    sp_session_player_play(spotifyService->spotifySession, 0);
    audio_fifo_flush(&g_audiofifo);
    player->isPaused = true;
  };
  spotifyService->executeSpotifyAPIcall(cb);
	return Undefined();
}

Handle<Value> Player::stop(const Arguments& args) {
	//DLOG(INFO) << "Player stopping";
  auto cb = [=] () { 
    sp_session_player_unload(spotifyService->spotifySession);
  };
  spotifyService->executeSpotifyAPIcall(cb);
  return Undefined();
}

Handle<Value> Player::resume(const Arguments& args) {
  //DLOG(INFO) << "Player resuming";
  Player* player = node::ObjectWrap::Unwrap<Player>(args.This());
	auto cb = [=] () { 
    if(player->isPaused) {
      sp_session_player_play(spotifyService->spotifySession, 1);
      player->isPaused = false;
    }
  };
  spotifyService->executeSpotifyAPIcall(cb);
  return Undefined();
}

Handle<Value> Player::play(const Arguments& args) {
  spotify::framesReceived = 0;
  spotify::currentSecond = 0;
  //DLOG(INFO) << "Player starting playing";
  Player* player = node::ObjectWrap::Unwrap<Player>(args.This());
  Track* track = node::ObjectWrap::Unwrap<Track>(args[0]->ToObject());
  auto cb = [=] () {
    sp_session_player_load(spotifyService->spotifySession, track->spotifyTrack);
    sp_session_player_play(spotifyService->spotifySession, 1);
  };
  player->track = track;
  spotifyService->executeSpotifyAPIcall(cb);
  return Undefined();
}

void Player::setCurrentSecond(int _currentSecond) {
  currentSecond = _currentSecond;
  call(PLAYER_SECOND_IN_SONG);
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
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Player");
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "play", play);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "pause", pause);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "resume", resume);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "stop", stop);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getCurrentlyPlayingData", getCurrentlyPlayingData);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("currentSecond"), &getCurrentSecond, emptySetter);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}
