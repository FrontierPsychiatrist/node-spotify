/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#include "Player.h"

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

Player::Player() : currentSecond(0), isPaused(false), isLoading(false),
  loadingTrack(nullptr), nodeObject(nullptr) {};

void Player::stop() {
  sp_session_player_unload(application->session);
}

void Player::pause() {
  sp_session_player_play(application->session, 0);
  audio_fifo_flush(&application->audio_fifo);
  isPaused = true;
}

void Player::resume() {
 if(isPaused) {
    sp_session_player_play(application->session, 1);
    isPaused = false;
  }
}

void Player::play(std::shared_ptr<Track> track) {
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
