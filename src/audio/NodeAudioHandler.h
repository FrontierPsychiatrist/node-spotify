#ifndef NODEAUDIOHANDLER_H
#define NODEAUDIOHANDLER_H

#include "AudioHandler.h"

#include <nan.h>
#include <uv.h>
#include <memory>

#include <node_version.h>

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
#if NODE_VERSION_AT_LEAST(0, 11, 0) // TODO 11 or 12?
  static void musicTimeout(uv_timer_t* timer);
#else
  static void musicTimeout(uv_timer_t* timer, int status);
#endif
  bool callMusicDeliveryCallback(audio_fifo_data_t* audioData);
};

#endif // NODEAUDIOHANDLER_H
