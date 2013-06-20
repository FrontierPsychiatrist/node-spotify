#include "Player.h"
#include "Track.h"
#include "PlaylistContainer.h"
#include "../SpotifyService/SpotifyService.h"
#include "base64.h"

#include <glog/logging.h>

extern SpotifyService* spotifyService;
extern PlaylistContainer* playlistContainer;
void imageLoadedCallback(sp_image* image, void* userdata);

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
  Player* player = node::ObjectWrap::Unwrap<Player>(args.This());
  int playlistId = args[0]->ToInteger()->Value();
  player->currentTrackPosition = args[1]->ToInteger()->Value();
  player->currentPlaylist = playlistContainer->getPlaylists()[playlistId];
  DLOG(INFO) << "Will play track " << player->currentTrackPosition << " on playlist " << playlistId;
  return Player::simpleCall(args, &Player::changeAndPlayTrack);
}

/**
 * Changes the currentTrack to the track at currentTrackPosition, gets all Metadata for the track and plays it
 * May only be called from the spotify thread
 **/
void Player::changeAndPlayTrack() {
  currentTrack = currentPlaylist->getTracks()[currentTrackPosition];
  const byte* coverId = sp_album_cover(currentTrack->album->spAlbum, SP_IMAGE_SIZE_NORMAL);
  sp_image* image = sp_image_create(spotifyService->spotifySession, coverId);
  if(sp_image_is_loaded(image)) {
    DLOG(INFO) << "Image for new track is already loaded";
    processImage(image);
    sp_image_release(image);
  } else {
    DLOG(INFO) << "Need to load image";
    sp_image_add_load_callback(image, &imageLoadedCallback, this);
  }
  //getCurrentTrackName
  //...
  //this->call(PLAYER_DATA_CHANGED)
  spotifyPlay();
}

void imageLoadedCallback(sp_image* image, void* userdata) {
  ((Player*)userdata)->processImage(image);
  sp_image_remove_load_callback(image, &imageLoadedCallback, userdata);
  sp_image_release(image);
}

/**
 * Reads raw image data from spotify, converts it to base64 char* and saves it in the player.
 * May only be called from the spotify thread
 **/
void Player::processImage(sp_image* image) {
  DLOG(INFO) << "Processing image data";
  size_t imageSize;
  int base64Size;
  const void* imageData = sp_image_data(image, &imageSize);
  this->currentAlbumCoverBase64 = base64(imageData, (int)imageSize, &base64Size);
  //this->call(PLAYER_DATA_CHANGED)
}

Handle<Value> Player::getCurrentTrack(const Arguments& args) {
  HandleScope scope;
  Player* player = node::ObjectWrap::Unwrap<Player>(args.This());
  return scope.Close(player->currentTrack->getV8Object());
}

void Player::spotifyPause() {
  sp_session_player_play(spotifyService->spotifySession, 0);
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
  DLOG(INFO) << "Calling sp_session_player_play";
  sp_session_player_load(spotifyService->spotifySession, currentTrack->spotifyTrack);
  sp_session_player_play(spotifyService->spotifySession, 1);
}

void Player::nextTrack() {
  currentTrackPosition++;
  if( currentTrackPosition < (int)currentPlaylist->getTracks().size()) {
    changeAndPlayTrack();
  }
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
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getCurrentTrack", getCurrentTrack);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}
