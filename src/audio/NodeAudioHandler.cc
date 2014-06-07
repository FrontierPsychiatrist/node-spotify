#include "NodeAudioHandler.h"
#include "../objects/spotify/Spotify.h"
#include "../Application.h"
#include "../common_macros.h"

#include <node_buffer.h>

using namespace v8;

extern Application* application;

NodeAudioHandler::NodeAudioHandler(Handle<Function> _musicDeliveryCallback) :
  AudioHandler(), musicDeliveryCallback(_musicDeliveryCallback), needMoreData(true) {
  uv_async_init(uv_default_loop(), &musicCallbackHandle, NodeAudioHandler::handleMusicInNodeThread);
  musicCallbackHandle.data = this;
}

NodeAudioHandler::~NodeAudioHandler() {

}

//helper function to free audioData after being copied into the node.js buffer
static void free_data(char* data, void* hint) {
  free(hint);
}

void NodeAudioHandler::handleMusicInNodeThread(uv_async_t* handle, int status) {
  //for perfomance reasons make the key symbols static.
  static Local<String> numberOfSamplesKey = String::NewSymbol("numberOfSamples");
  static Local<String> sampleRateKey = String::NewSymbol("sampleRate");
  static Local<String> channelsKey = String::NewSymbol("channels");

  NodeAudioHandler* audioHandler = static_cast<NodeAudioHandler*>(handle->data);
  audio_fifo_t *audioFifo = &audioHandler->audioFifo;
  audio_fifo_data_t* audioData;

  while( audioHandler->needMoreData && (audioData = audio_get(audioFifo)) ) {
    size_t size = audioData->numberOfSamples * sizeof(int16_t) * audioData->channels;
    node::Buffer *slowBuffer = node::Buffer::New((char*)audioData->samples, size, free_data, audioData);

    Local<Object> globalObj = Context::GetCurrent()->Global();
    Local<Function> ctor = Local<Function>::Cast(globalObj->Get(String::New("Buffer")));
    Handle<Value> constructorArgs[3] = { slowBuffer->handle_, Integer::New(size), Integer::New(0)};
    Handle<Object> actualBuffer = ctor->NewInstance(3, constructorArgs);

    actualBuffer->Set(numberOfSamplesKey, Integer::New(audioData->numberOfSamples));
    actualBuffer->Set(sampleRateKey, Integer::New(audioData->sampleRate));
    actualBuffer->Set(channelsKey, Integer::New(audioData->channels));

    int argc = 2;
    Handle<Value> argv[] = { Undefined(), actualBuffer };
    Handle<Value> bufferFilled = audioHandler->musicDeliveryCallback->Call(globalObj, argc, argv);
    audioHandler->needMoreData = bufferFilled->ToBoolean()->BooleanValue();
  }
}

void NodeAudioHandler::afterMusicDelivery(const sp_audioformat *format) {
  if(audioFifo.samplesInQueue > format->sample_rate) {
    uv_async_send(&musicCallbackHandle);
  }
}

bool NodeAudioHandler::dataNeeded() {
  return needMoreData;
}

Handle<Value> NodeAudioHandler::setNeedMoreData(const Arguments &args) {
  HandleScope scope;
  if(args.Length() < 1 || !args[0]->IsBoolean()) {
    return scope.Close(V8_EXCEPTION("setNeedMoreData needs a boolean as its first argument."));
  }
  Spotify* spotify = static_cast<Spotify*>(sp_session_userdata(application->session));
  NodeAudioHandler* audioHandler = static_cast<NodeAudioHandler*>(spotify->audioHandler.get());
  bool needMoreData = args[0]->ToBoolean()->BooleanValue();
  audioHandler->needMoreData = needMoreData;
  return scope.Close(Undefined());
}
