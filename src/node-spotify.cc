#include "Application.h"
#include "exceptions.h"
#include "objects/node/NodeSpotify.h"
#include "objects/node/NodePlaylist.h"
#include "objects/node/NodePlaylistContainer.h"
#include "objects/node/NodeTrack.h"
#include "objects/node/NodeTrackExtended.h"
#include "objects/node/NodePlayer.h"
#include "objects/node/NodeAlbum.h"
#include "objects/node/NodeArtist.h"
#include "objects/node/NodeSearch.h"
#include "objects/node/NodePlaylistFolder.h"
#include "objects/node/NodeUser.h"

#include <node.h>
#include <nan.h>

#ifdef NODE_SPOTIFY_NATIVE_SOUND
#include "audio/NativeAudioHandler.h"
#endif

Application* application;

static Handle<Object> getInternal() {
  Local<Object> internal = NanNew<Object>();
  Local<Object> protos = NanNew<Object>();
  protos->Set(NanNew<String>("Playlist"), NodePlaylist::getConstructor());
  protos->Set(NanNew<String>("Track"), NodeTrack::getConstructor());
  protos->Set(NanNew<String>("TrackExtended"), NodeTrackExtended::getConstructor());
  protos->Set(NanNew<String>("PlaylistContainer"), NodePlaylistContainer::getConstructor());
  protos->Set(NanNew<String>("Artist"), NodeArtist::getConstructor());
  protos->Set(NanNew<String>("Album"), NodeAlbum::getConstructor());
  protos->Set(NanNew<String>("User"), NodeUser::getConstructor());
  protos->Set(NanNew<String>("PlaylistFolder"), NodePlaylistFolder::getConstructor());
  internal->Set(NanNew<String>("protos"), protos);

  return internal;
}

NAN_METHOD(CreateNodespotify) {
  NanScope();

  //initiate the javascript ctors and prototypes
  NodePlaylist::init();
  NodeTrack::init();
  NodeTrackExtended::init();
  NodeArtist::init();
  NodePlayer::init();
  NodeAlbum::init();
  NodeSearch::init();
  NodeSpotify::init();
  NodePlaylistFolder::init();
  NodePlaylistContainer::init();
  NodeUser::init();

  //initialize application struct
  application = new Application();

#ifdef NODE_SPOTIFY_NATIVE_SOUND
  application->audioHandler = std::unique_ptr<AudioHandler>(new NativeAudioHandler());
#endif

  v8::Handle<v8::Object> options;
  if(args.Length() < 1) {
    options = NanNew<Object>();
  } else {
    if(!args[0]->IsObject()) {
      return NanThrowError("Please provide an object to the node-spotify initializer function");
    }
    options = args[0]->ToObject();
  }

  NodeSpotify* nodeSpotify;
  try {
    nodeSpotify = new NodeSpotify(options);
  } catch (const FileException& e) {
    return NanThrowError("Appkey file not found");
  } catch (const SessionCreationException& e) {
    return NanThrowError(e.message.c_str());
  }
  v8::Handle<Object> spotifyObject = nodeSpotify->createInstance();

  //Set some fields on the nodeSpotify object
  spotifyObject->Set(NanNew<String>("Search"), NodeSearch::getConstructor());//TODO: this is ugly but didn't work when done in the NodeSpotify ctor
  spotifyObject->Set(NanNew<String>("internal"), getInternal());
  application->player = std::make_shared<Player>();
  NodePlayer* nodePlayer = new NodePlayer(application->player);
  spotifyObject->Set(NanNew<String>("player"), nodePlayer->createInstance());
  NanReturnValue(spotifyObject);
};

static void init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module) {
  module->Set(NanNew<String>("exports"), NanNew<FunctionTemplate>(CreateNodespotify)->GetFunction());
}

NODE_MODULE(nodespotify, init)
