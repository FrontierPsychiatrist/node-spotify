#include <node.h>
#include <v8.h>

#include "SpotifyService/SpotifyService.h"
#include "NodeCallback.h"
#include "spotify/StaticCallbackSetter.h"
#include "spotify/Playlist.h"
#include "spotify/Track.h"
#include "spotify/Player.h"

extern "C" {
#include "audio/audio.h"
}

using namespace v8;

SpotifyService* spotifyService;
extern PlaylistContainer* playlistContainer; //defined in SessionCallbacks.cc
extern audio_fifo_t g_audiofifo;

Handle<Value> login(const Arguments& args) {
  HandleScope scope;
  String::Utf8Value v8User(args[0]->ToString());
  String::Utf8Value v8Password(args[1]->ToString());
  std::string user(*v8User);
  std::string password(*v8Password);
  spotifyService->login(user, password);
  return scope.Close(Undefined());
}

Handle<Value> logout(const Arguments& args) {
  HandleScope scope;
  Callback<SpotifyService>* logoutCallback = new Callback<SpotifyService>(spotifyService, &SpotifyService::logout);
  spotifyService->executeSpotifyAPIcall(logoutCallback);
  return scope.Close(Undefined());
}

Handle<Value> ready(const Arguments& args) {
  HandleScope scope;
  Handle<Function> fun = Handle<Function>::Cast(args[0]);
  Persistent<Function> p = Persistent<Function>::New(fun);
  PlaylistContainer::setContainerLoadedCallback(p);
  return scope.Close(Undefined());
}

Handle<Value> getPlaylists(const Arguments& args) {
  HandleScope scope;
  std::vector<Playlist*> playlists = playlistContainer->getPlaylists();
  Local<Array> nPlaylists = Array::New(playlists.size());
  for(int i = 0; i < (int)playlists.size(); i++) {
    nPlaylists->Set(Number::New(i), playlists[i]->getV8Object());
  }
  return scope.Close(nPlaylists);
}

/**
 * Handle a NodeCallback sent to this thread. 
 * The handle should contain a NodeCallback struct
 **/
void resolveCallback(uv_async_t* handle, int status) {
  HandleScope scope;
  NodeCallback* nodeCallback = (NodeCallback*)handle->data;
  Handle<Function>* fun = nodeCallback->function;

  const unsigned int argc = 0;
  Local<Value> argv[argc] = { };
  if(!(*fun).IsEmpty() && (*fun)->IsCallable()) {
    //Check if an object is attached to the struct and if, use it as the scope.
    if(nodeCallback->object.IsEmpty())
      (*fun)->Call(Context::GetCurrent()->Global(), argc, argv);
    else
      (*fun)->Call(nodeCallback->object, argc, argv);
  }
  scope.Close(Undefined());
}

void init(Handle<Object> target) {
  Playlist::init(target);
  Track::init(target);
  Artist::init(target);
  Player::init(target);
  StaticCallbackSetter<Playlist>::init(target, "playlists");
  audio_init(&g_audiofifo);
  spotifyService = new SpotifyService();
  Player* player = new Player(&spotifyService->callNodeThread);
  target->Set(String::NewSymbol("login"),
              FunctionTemplate::New(login)->GetFunction());
  target->Set(String::NewSymbol("logout"),
              FunctionTemplate::New(logout)->GetFunction());
  target->Set(String::NewSymbol("getPlaylists"),
              FunctionTemplate::New(getPlaylists)->GetFunction());
  target->Set(String::NewSymbol("ready"),
              FunctionTemplate::New(ready)->GetFunction());
  target->Set(String::NewSymbol("player"), player->getV8Object());
   
  //Initialize waiting for callbacks from the spotify thread
  uv_async_init(uv_default_loop(), &spotifyService->callNodeThread, resolveCallback);
}
NODE_MODULE(spotify, init)
