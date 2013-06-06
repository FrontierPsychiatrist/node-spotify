#include "Playlist.h"
#include "Track.h"

#include "../events.h"
#include "../SpotifyService/SpotifyService.h"

#include <vector>

extern SpotifyService* spotifyService;

std::vector<Track*> Playlist::getTracks() {
  pthread_mutex_lock(&lockingMutex);
  if(!tracksLoaded) {
    Callback<Playlist>* loadTracksCallback = new Callback<Playlist>(this, &Playlist::loadTracks);
    spotifyService->executeSpotifyAPIcall(loadTracksCallback);
    this->wait();
	}
  pthread_mutex_unlock(&lockingMutex);
	return tracks;
}

void Playlist::setName(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  Playlist* playlist = node::ObjectWrap::Unwrap<Playlist>(info.Holder());
  pthread_mutex_lock(&playlist->lockingMutex);
  String::Utf8Value v8Str(value);
  playlist->name = *v8Str;
  pthread_mutex_unlock(&playlist->lockingMutex);
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
  
  pthread_mutex_lock(&playlist->lockingMutex);
  if(!playlist->tracksLoaded) {
    Callback<Playlist>* loadTracksCallback = new Callback<Playlist>(playlist, &Playlist::loadTracks);
    spotifyService->executeSpotifyAPIcall(loadTracksCallback);
    playlist->wait();
  }
  pthread_mutex_unlock(&playlist->lockingMutex);
  Local<Array> nTracks = Array::New(playlist->tracks.size());
  for(int i = 0; i < (int)playlist->tracks.size(); i++) {
    nTracks->Set(Number::New(i), playlist->tracks[i]->getV8Object() );
  }
  return scope.Close(nTracks);
}

void Playlist::init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("Playlist"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, setName);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("id"), getId, emptySetter);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getTracks", getTracks);

  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}

void Playlist::loadTracks() {
  for(int i = 0; i < sp_playlist_num_tracks(playlist); ++i) {
    sp_track* spTrack = sp_playlist_track(playlist, i);
    const char* trackName = sp_track_name(spTrack);

    int numArtists = sp_track_num_artists(spTrack);
    std::vector<Artist*> artists;
    for(int i = 0; i < numArtists; i++) {
      sp_artist* spArtist = sp_track_artist(spTrack, i);
      Artist* artist = Artist::getArtist(spArtist);
      if(artist == 0) {//TODO: refactor this mess
        const char* artistName = sp_artist_name(spArtist);
        artist = new Artist(std::string(artistName), spArtist);
        Artist::putArtist(artist);
      }
      artists.push_back(artist);
    }

    Track* track = new Track(spTrack, asyncHandle, std::string(trackName), artists);
    tracks.push_back(track);
  }
  tracksLoaded = true;
  done();
}
