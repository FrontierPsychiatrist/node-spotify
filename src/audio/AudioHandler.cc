#include "AudioHandler.h"
#include "../objects/spotify/Spotify.h"

#include <stdlib.h>

AudioHandler::AudioHandler() {
  audio_init(&audioFifo);
}

AudioHandler::~AudioHandler() {
  audio_stop(&audioFifo);
}

int AudioHandler::musicDelivery(sp_session *session, const sp_audioformat *format, const void *frames, int num_frames) {
  Spotify* spotify = static_cast<Spotify*>(sp_session_userdata(session));
  if(num_frames == 0 || !spotify->audioHandler->dataNeeded()) {
    return 0;
  }

  audio_fifo_t* audioFifo = &spotify->audioHandler->audioFifo;
  uv_mutex_lock(&audioFifo->audioQueueMutex);

  //If there is more than one second worth of samples in the queue don't buffer more
  if(audioFifo->samplesInQueue > format->sample_rate) {
    uv_mutex_unlock(&audioFifo->audioQueueMutex);
    return 0;
  }

  size_t size = num_frames * sizeof(int16_t) * format->channels;
  audio_fifo_data_t* audioData = (audio_fifo_data_t*)malloc(sizeof(*audioData) + size);
  memcpy(audioData->samples, frames, size);

  audioData->numberOfSamples = num_frames;
  audioData->sampleRate = format->sample_rate;
  audioData->channels = format->channels;

  TAILQ_INSERT_TAIL(&audioFifo->queue, audioData, link);
  audioFifo->samplesInQueue += num_frames;

  spotify->audioHandler->afterMusicDelivery(format);

  uv_mutex_unlock(&audioFifo->audioQueueMutex);

  return num_frames;
}

void AudioHandler::getAudioBufferStats(sp_session* session, sp_audio_buffer_stats* stats) {
  //TODO:this crashes when switching the audio handler in between playback because the mutex gets destroyed.
  Spotify* spotify = static_cast<Spotify*>(sp_session_userdata(session));
  audio_fifo_t *audioFifo = &spotify->audioHandler->audioFifo;
  uv_mutex_lock(&audioFifo->audioQueueMutex);
  stats->samples = audioFifo->samplesInQueue;
  stats->stutter = 0;
  uv_mutex_unlock(&audioFifo->audioQueueMutex);
}
