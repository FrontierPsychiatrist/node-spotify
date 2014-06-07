#ifndef NODEAUDIOHANDLER_H
#define NODEAUDIOHANDLER_H

#include "AudioHandler.h"

#include <v8.h>
#include <uv.h>

/**
 * @brief The NodeAudioHandler class
 * Handles audio data by calling a user provided Javascript callback with it.
 */
class NodeAudioHandler : public AudioHandler {
public:
  NodeAudioHandler(v8::Handle<v8::Function> musicDeliveryCallback);
  ~NodeAudioHandler();
  static v8::Handle<v8::Value> setNeedMoreData(const v8::Arguments& args);
protected:
  void afterMusicDelivery(const sp_audioformat* format);
  bool dataNeeded();
private:
  v8::Handle<v8::Function> musicDeliveryCallback;
  uv_async_t musicCallbackHandle;
  bool needMoreData;
  static void handleMusicInNodeThread(uv_async_t* handle, int status);
};

#endif // NODEAUDIOHANDLER_H
