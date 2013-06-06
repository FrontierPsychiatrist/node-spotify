#include <libspotify/api.h>
#include "../Playlist.h"
#include "../SpotifyService/SpotifyService.h"

extern SpotifyService* spotifyService;
extern PlaylistContainer* playlistContainer;

Handle<Value> Player::pause(const Arguments& args) {
	return Player::simpleCall(args, &Player::spotifyPause);
}

Handle<Value> Player::stop(const Arguments& args) {
	return Player::simpleCall(args, &Player::spotifyStop);
}

Handle<Value> Player::resume(const Arguments& args) {
	return Player::simpleCall(args, &Player::spotifyResume);
}

Handle<Value> Player::play(const Arguments& args) {
  //TODO: check if args contains an integer and if so try to play from that track on
	return Player::simpleCall(args, &Player::spotifyPlay);
}

Handle<Value> Player::addTracks(const Arguments& args) {
	HandleScope scope;
	Player* player = node::ObjectWrap::Unwrap<Player>(args.This());
	player->internAddTracks( args[0]->ToInteger().Value(), arg[1]->ToInteger().Value());
	return scope.Close(Undefined());
}

void Player::spotifyPause() {
	sp_session_player_play(spotifyService->getSpotifySession(), 0);
	isPaused = true;
}

void Player::spotifyResume() {
	if(isPaused) {
		sp_session_player_play(spotifyService->getSpotifySession(), 1);
		isPaused = false;
	}
}

void Player::spotifyStop() {
	sp_session_player_unload(spotifyService->getSpotifySession());
}

void Player::spotifyPlay() {
  if(playQueue.size() > 1)
    sp_session_player_load(spotifyService->getSpotifySession(), playQueue[0]);
}

void Playlist::internAddTracks(int playlistId, int trackId) {
	std::vector<Playlist*> playlists = playlistContainer->getPlaylists();
	Playlist* playlist = playlists[playlistId];
	std::vector<Track*> tracks = playlist->getTracks();
	playQueue.insert(playQueue.begin() + currentTrackPosition, tracks.begin() + trackId, tracks.end());	
}

void Player::init(Handle<Object> target) {
	HandleScope scope;
	Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
	constructorTemplate->SetClassName(String::NewSymbol("Player"));
	constructorTemplate->InstanceTemplate->SetInternalFieldCount(1);
	
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "play", play);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "pause", pause);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "resume", resume);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "stop", stop);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "addTracks", addTracks);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close();
}