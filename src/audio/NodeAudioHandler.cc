#include "NodeAudioHandler.h"
#include "../objects/spotify/Spotify.h"
#include "../Application.h"

#include <node_buffer.h>

using namespace v8;

extern Application* application;

NodeAudioHandler::NodeAudioHandler(Handle<Function> _musicDeliveryCallback) :
  AudioHandler(), needMoreData(true), stopped(false), musicTimerRepeat(20) {
  musicDeliveryCallback.SetFunction(_musicDeliveryCallback);
  uv_timer_init(uv_default_loop(), &musicTimer);
  musicTimer.data = this;
  uv_timer_start(&musicTimer, &musicTimeout, 0, musicTimerRepeat);
}

NodeAudioHandler::~NodeAudioHandler() {
  uv_timer_stop(&musicTimer);
}

#if NODE_VERSION_AT_LEAST(0, 11, 0)
void NodeAudioHandler::musicTimeout(uv_timer_t* timer) {
#else
void NodeAudioHandler::musicTimeout(uv_timer_t* timer, int status) {
#endif
  NodeAudioHandler* audioHandler = static_cast<NodeAudioHandler*>(timer->data);
  audio_fifo_t *audioFifo = &audioHandler->audioFifo;
  audio_fifo_data_t* audioData;

  if( !audioHandler->stopped && audioHandler->needMoreData && (audioData = audio_get(audioFifo)) ) {
    // The audio data will be freed in a callback that is set up in this method.
    audioHandler->needMoreData = audioHandler->callMusicDeliveryCallback(audioData);
  }
}

/**
 * @brief free_data Free the audio data after buffer initialization.
 */
static void free_data(char* data, void* hint) {
  audio_fifo_data_t* audioData = static_cast<audio_fifo_data_t*>(hint);
  assert((char*)audioData->samples == data);
  free(audioData);
}

/**
 * @brief NodeAudioHandler::callMusicDeliveryCallback
 * Call the user provided JavaScript callback with the audio data.
 * @param audioData
 * @return true if the callback needs more data.
 */
bool NodeAudioHandler::callMusicDeliveryCallback(audio_fifo_data_t* audioData) {
  Nan::HandleScope scope;
  static Local<String> numberOfSamplesKey = Nan::New<String>("numberOfSamples").ToLocalChecked();
  static Local<String> sampleRateKey = Nan::New<String>("sampleRate").ToLocalChecked();
  static Local<String> channelsKey = Nan::New<String>("channels").ToLocalChecked();

  size_t size = audioData->numberOfSamples * sizeof(int16_t) * audioData->channels;
  Local<Object> actualBuffer = Nan::NewBuffer((char*)audioData->samples, size, free_data, audioData).ToLocalChecked();
  //node::Buffer *slowBuffer = node::Buffer::New((char*)audioData->samples, size, free_data, audioData);

  //Local<Object> globalObj = Context::GetCurrent()->Global();
  //Local<Function> ctor = Local<Function>::Cast(globalObj->Get(String::New("Buffer")));
  //Handle<Value> constructorArgs[3] = { slowBuffer->handle_, Integer::New(size), Integer::New(0)};
  //Handle<Object> actualBuffer = ctor->NewInstance(3, constructorArgs);

  actualBuffer->Set(numberOfSamplesKey, Nan::New<Integer>(audioData->numberOfSamples));
  actualBuffer->Set(sampleRateKey, Nan::New<Integer>(audioData->sampleRate));
  actualBuffer->Set(channelsKey, Nan::New<Integer>(audioData->channels));

  int argc = 2;
  Handle<Value> argv[] = { Nan::Undefined(), actualBuffer }; // TODO set an error here if the Nan::NewBuffer was not created correctly?
  Handle<Value> bufferFilled = musicDeliveryCallback.Call(argc, argv);
  return bufferFilled->ToBoolean()->BooleanValue();
}

void NodeAudioHandler::afterMusicDelivery(const sp_audioformat *format) { }

bool NodeAudioHandler::dataNeeded() {
  return needMoreData;
}

void NodeAudioHandler::setStopped(bool _stopped) {
  stopped = _stopped;
  AudioHandler::setStopped(_stopped);
}

NAN_METHOD(NodeAudioHandler::setNeedMoreData) {
  if(info.Length() < 1 || !info[0]->IsBoolean()) {
    Nan::ThrowError("setNeedMoreData needs a boolean as its first argument.");
    return;
  }
  NodeAudioHandler* audioHandler = static_cast<NodeAudioHandler*>(application->audioHandler.get());
  bool needMoreData = info[0]->ToBoolean()->BooleanValue();
  audioHandler->needMoreData = needMoreData;
  info.GetReturnValue().SetUndefined();
}
