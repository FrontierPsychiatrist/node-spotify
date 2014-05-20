/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#include "Application.h"
#include "exceptions.h"
#include "common_macros.h"
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
#include "audio/audio.h"

#include <node.h>
#include <v8.h>

Application* application;

v8::Handle<v8::Value> CreateNodespotify(const v8::Arguments& args) {
  v8::HandleScope scope;

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
  audio_init(&application->audio_fifo);

  //configure and create spotify session
  v8::Handle<v8::Object> options;
  if(args.Length() < 1) {
    options = v8::Object::New();
  } else {
    if(!args[0]->IsObject()) {
      return scope.Close(V8_EXCEPTION("Please provide an object to the node-spotify initializer function"));
    }
    options = args[0]->ToObject();
  }

  //Create nodeSpotify object
  NodeSpotify* nodeSpotify;
  try {
    nodeSpotify = new NodeSpotify(options);
  } catch (const FileException& e) {
    return scope.Close(V8_EXCEPTION("Appkey file not found"));
  } catch (const SessionCreationException& e) {
    return scope.Close(V8_EXCEPTION(e.message.c_str()));
  }
  v8::Handle<Object> spotifyObject = nodeSpotify->getV8Object();

  //Set some fields on the nodeSpotify object
  spotifyObject->Set(v8::String::NewSymbol("Search"), NodeSearch::getConstructor());//TODO: this is ugly but didn't work when done in the NodeSpotify ctor
  NodePlayer* nodePlayer = new NodePlayer();
  spotifyObject->Set(v8::String::NewSymbol("player"), nodePlayer->getV8Object());
  return scope.Close(spotifyObject);
};

static void init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module) {
  module->Set(v8::String::NewSymbol("exports"), v8::FunctionTemplate::New(CreateNodespotify)->GetFunction());
}

NODE_MODULE(nodespotify, init)