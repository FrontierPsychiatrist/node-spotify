#include "Track.h"

#include "../SpotifyService/SpotifyService.h"
#include "../Callback.h"

Handle<Value> Track::getName(Local<String> property, const AccessorInfo& info) {
  Track* track = node::ObjectWrap::Unwrap<Track>(info.Holder());
  return String::New(track->name.c_str());
}

Handle<Value> Track::getArtists(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  Track* track = node::ObjectWrap::Unwrap<Track>(info.Holder());
  Local<Array> jsArtists = Array::New(track->artists.size());
  for(int i = 0; i < (int)track->artists.size(); i++) {
    jsArtists->Set(Number::New(i), track->artists[i]->getV8Object() );
  }
  return scope.Close(jsArtists); 
}

Handle<Value> Track::getAlbum(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  Track* track = node::ObjectWrap::Unwrap<Track>(info.Holder());
  return scope.Close(track->album->getV8Object());
}

Handle<Value> Track::play(const Arguments& args) {
  HandleScope scope;
  Track* track = node::ObjectWrap::Unwrap<Track>(args.This());
  Callback<Track>* cb = new Callback<Track>(track, &Track::spotifyPlay);
  spotifyService->executeSpotifyAPIcall(cb);
  return scope.Close(Undefined());
}

void Track::spotifyPlay() {
  sp_session_player_load(spotifyService->spotifySession, this->spotifyTrack);
  sp_session_player_play(spotifyService->spotifySession, 1);
}

void Track::init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("Track"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("artists"), getArtists, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("album"), getAlbum, emptySetter);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "play", play);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
}
