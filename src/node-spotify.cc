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
  Local<Object> internal = Nan::New<Object>();
  Local<Object> protos = Nan::New<Object>();
  protos->Set(Nan::New<String>("Playlist").ToLocalChecked(), NodePlaylist::getConstructor());
  protos->Set(Nan::New<String>("Track").ToLocalChecked(), NodeTrack::getConstructor());
  protos->Set(Nan::New<String>("TrackExtended").ToLocalChecked(), NodeTrackExtended::getConstructor());
  protos->Set(Nan::New<String>("PlaylistContainer").ToLocalChecked(), NodePlaylistContainer::getConstructor());
  protos->Set(Nan::New<String>("Artist").ToLocalChecked(), NodeArtist::getConstructor());
  protos->Set(Nan::New<String>("Album").ToLocalChecked(), NodeAlbum::getConstructor());
  protos->Set(Nan::New<String>("User").ToLocalChecked(), NodeUser::getConstructor());
  protos->Set(Nan::New<String>("PlaylistFolder").ToLocalChecked(), NodePlaylistFolder::getConstructor());
  internal->Set(Nan::New<String>("protos").ToLocalChecked(), protos);

  return internal;
}

NAN_METHOD(CreateNodespotify) {
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
  if(info.Length() < 1) {
    options = Nan::New<Object>();
  } else {
    if(!info[0]->IsObject()) {
      Nan::ThrowError("Please provide an object to the node-spotify initializer function");
      return;
    }
    options = info[0]->ToObject();
  }

  NodeSpotify* nodeSpotify;
  try {
    nodeSpotify = new NodeSpotify(options);
  } catch (const FileException& e) {
    Nan::ThrowError("Appkey file not found");
    return;
  } catch (const SessionCreationException& e) {
    Nan::ThrowError(e.message.c_str());
    return;
  }
  v8::Handle<Object> spotifyObject = nodeSpotify->createInstance();

  //Set some fields on the nodeSpotify object
  spotifyObject->Set(Nan::New<String>("Search").ToLocalChecked(), NodeSearch::getConstructor());//TODO: this is ugly but didn't work when done in the NodeSpotify ctor
  spotifyObject->Set(Nan::New<String>("internal").ToLocalChecked(), getInternal());
  application->player = std::make_shared<Player>();
  NodePlayer* nodePlayer = new NodePlayer(application->player);
  spotifyObject->Set(Nan::New<String>("player").ToLocalChecked(), nodePlayer->createInstance());
  info.GetReturnValue().Set(spotifyObject);
};

static void init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module) {
  Nan::SetMethod(module, "exports", CreateNodespotify);
}

NODE_MODULE(nodespotify, init)
