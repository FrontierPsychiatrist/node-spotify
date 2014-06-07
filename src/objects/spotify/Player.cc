#include "Player.h"
#include "Spotify.h"
#include "../../exceptions.h"
#include "../../Application.h"

extern "C" {
  #include "../../audio/audio.h"
}

/* REMOVE ME */
namespace spotify {
  extern int framesReceived;
  extern int currentSecond;
}

extern Application* application;

Player::Player() : currentSecond(0), isPaused(false), isLoading(false), loadingTrack(nullptr) {}

void Player::stop() {
  sp_session_player_unload(application->session);
}

void Player::pause() {
  sp_session_player_play(application->session, 0);
  Spotify* spotify = static_cast<Spotify*>(sp_session_userdata(application->session));
  //TODO: flush audio handler
  //audio_fifo_flush(&application->audio_fifo);
  isPaused = true;
}

void Player::resume() {
 if(isPaused) {
    sp_session_player_play(application->session, 1);
    isPaused = false;
  }
}

void Player::play(std::shared_ptr<Track> track) {
#ifndef NODE_SPOTIFY_NATIVE_SOUND
  //If node-spotify is compiled without native sound we have to check if the user registered a nodejs audio handler.
  Spotify* spotify = static_cast<Spotify*>(sp_session_userdata(application->session));
  if(!spotify->audioHandler) {
    throw NoAudioHandlerException();
  }
#endif
  spotify::framesReceived = 0;
  spotify::currentSecond = 0;
  sp_error error = sp_session_player_load(application->session, track->track);
  if(error == SP_ERROR_IS_LOADING) {
    isLoading = true;
    loadingTrack = track->track;
  } else if (error == SP_ERROR_TRACK_NOT_PLAYABLE) {
    throw TrackNotPlayableException();
  } else {
    sp_session_player_play(application->session, 1);
  }
}

void Player::retryPlay() {
  sp_error error = sp_track_error(loadingTrack);
  if(error != SP_ERROR_IS_LOADING) {
    sp_session_player_load(application->session, loadingTrack);
    sp_session_player_play(application->session, 1);
    isLoading = false;
    loadingTrack = nullptr;
  }
}

void Player::seek(int second) {
  sp_session_player_seek(application->session, second*1000);
  spotify::currentSecond = second;
}

void Player::setCurrentSecond(int second) {
  currentSecond = second;
}

std::shared_ptr<Player> Player::instance = std::make_shared<Player>();
