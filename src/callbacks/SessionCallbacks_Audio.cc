/**
Everything audio related in Session Callbacks.
**/
#include "SessionCallbacks.h"
#include "../objects/spotify/Player.h"
#include "../Application.h"
#include "../utils/V8Utils.h"

#include <v8.h>
#include <node_buffer.h>
#include <stdlib.h>

#include <iostream>

using namespace v8;

extern Application* application;


namespace spotify {
int framesReceived = 0;
int currentSecond = 0;
}

void SessionCallbacks::end_of_track(sp_session* session) {
  sp_session_player_unload(application->session);
  spotify::framesReceived = 0;
  spotify::currentSecond = 0;
  
  V8Utils::callV8FunctionWithNoArgumentsIfHandleNotEmpty(endOfTrackCallback);

/*  if(true /* replace this with if javascript callback used ) {
    std::cout << "End of track" << std::endl;
    //Insert an empty audio data to signalize end of track
    audio_fifo_t* audioFifo = &application->audio_fifo;
    uv_mutex_lock(&audioFifo->audioQueueMutex);
    audio_fifo_data_t* audioData = (audio_fifo_data_t*)malloc(sizeof(audio_fifo_data_t));
    audioData->numberOfSamples = -1;
    TAILQ_INSERT_TAIL(&audioFifo->queue, audioData, link);
    uv_mutex_unlock(&audioFifo->audioQueueMutex);
  }*/
}

/**
Save the second we are currently in while playing music
**/
void SessionCallbacks::sendTimer(int sample_rate) {
  if( spotify::framesReceived / sample_rate > 0) {
    spotify::currentSecond++;
    spotify::framesReceived = spotify::framesReceived - sample_rate;
    Player::instance->setCurrentSecond(spotify::currentSecond);
  }
}

void SessionCallbacks::start_playback(sp_session* session) {
}

void SessionCallbacks::stop_playback(sp_session* session) {

}
