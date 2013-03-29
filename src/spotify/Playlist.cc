#include "Playlist.h"
#include "../events.h"
#include "../Callback.h"

#include "../SpotifyService/SpotifyService.h"

#include <iostream>

extern SpotifyService* spotifyService;

void Playlist::setName(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(info.Holder());
  String::Utf8Value v8Str(value);
  playlist->name = *v8Str;
}

Handle<Value> Playlist::getName(Local<String> property, const AccessorInfo& info) {
  Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(info.Holder());
  return String::New(playlist->name.c_str());
}

Handle<Value> Playlist::getId(Local<String> property, const AccessorInfo& info) {
  Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(info.Holder());
  return Integer::New(playlist->id);
}

Handle<Value> Playlist::getTracks(const Arguments& args) {
  HandleScope scope;
  Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(args.This());

  if(playlist->tracks.size() == 0) {
    Callback<Playlist>* loadTracksCallback = new Callback<Playlist>(playlist, &Playlist::loadTracks);
    spotifyService->executeSpotifyAPIcall(loadTracksCallback);
  }

  Local<Array> nTracks = Array::New(playlist->tracks.size());
  for(int i = 0; i < (int)playlist->tracks.size(); i++) {
    nTracks->Set(Number::New(i), playlist->tracks[i].getV8Object() );
  }
  return scope.Close(nTracks);
}

void Playlist::init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("Playlist"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  constructorTemplate->InstanceTemplate()->SetAccessor(String::New("name"), getName, setName);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::New("id"), getId, emptySetter);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getTracks", getTracks);

  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
}

void Playlist::loadTracks() {
  for(int i = 0; i < sp_playlist_num_tracks(playlist); ++i) {
    sp_track* spTrack = sp_playlist_track(playlist, i);
    const char* trackName = sp_track_name(spTrack);
    Track track(spTrack, asyncHandle, std::string(trackName));
    tracks.push_back(track);
  }
}
