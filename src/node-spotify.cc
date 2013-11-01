#include "Application.h"
#include "objects/node/NodeSpotify.h"
#include "objects/node/NodePlaylist.h"
#include "objects/node/NodeTrack.h"
#include "objects/node/NodePlayer.h"
#include "objects/node/NodeAlbum.h"
#include "objects/node/NodeArtist.h"
#include "objects/node/NodeSearch.h"
#include "objects/node/StaticCallbackSetter.h"
#include "audio/audio.h"

#include <node.h>
#include <v8.h>

Application* application;

v8::Handle<v8::Value> CreateNodespotify(const v8::Arguments& args) {
  v8::HandleScope scope;

  //initiate the javascript ctors and prototypes
  NodePlaylist::init();
  NodeTrack::init();
  NodeArtist::init();
  NodePlayer::init();
  NodeAlbum::init();
  NodeSearch::init();
  NodeSpotify::init();

  application = new Application();
  audio_init(&application->audio_fifo);

  //configure and create spotify session
  v8::Handle<v8::Object> options;
  if(args.Length() < 1) {
    options = v8::Object::New();
  } else {
    if(!args[0]->IsObject()) {
      return scope.Close(v8::ThrowException(v8::Exception::Error(v8::String::New("Please provide an object to the node-spotify initializer function"))));
    }
    options = args[0]->ToObject();
  }
  NodeSpotify* nodeSpotify = new NodeSpotify(options);
  v8::Handle<Object> out = nodeSpotify->getV8Object();
  out->Set(v8::String::NewSymbol("Search"), NodeSearch::getConstructor());//TODO: this is ugly but didn't work when done in the NodeSpotify ctor
  out->Set(v8::String::NewSymbol("player"), NodePlayer::getInstance().getV8Object());
  StaticCallbackSetter<NodePlaylist>::init(out, "playlists");
  return scope.Close(out);
};

static void init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module) {
  module->Set(v8::String::NewSymbol("exports"), v8::FunctionTemplate::New(CreateNodespotify)->GetFunction());
}

NODE_MODULE(spotify, init)