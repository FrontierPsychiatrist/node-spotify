#ifndef NODEAUDIOHANDLER_H
#define NODEAUDIOHANDLER_H

#include "AudioHandler.h"

#include <nan.h>
#include <uv.h>
#include <memory>

/**
 * @brief The NodeAudioHandler class
 * Handles audio data by calling a user provided Javascript callback with it.
 */
class NodeAudioHandler : public AudioHandler {
public:
  NodeAudioHandler(std::unique_ptr<NanCallback> musicDeliveryCallback);
  ~NodeAudioHandler();
  void setStopped(bool stopped);
  static NAN_METHOD(setNeedMoreData);
protected:
  void afterMusicDelivery(const sp_audioformat* format);
  bool dataNeeded();
private:
  std::unique_ptr<NanCallback> musicDeliveryCallback;
  uv_timer_t musicTimer;
  bool needMoreData;
  bool stopped;
  int musicTimerRepeat;
  static void musicTimeout(uv_timer_t* timer);
  bool callMusicDeliveryCallback(audio_fifo_data_t* audioData);
};

#endif // NODEAUDIOHANDLER_H
