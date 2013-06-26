#include <node.h>
#include <v8.h>

#include "SpotifyService/SpotifyService.h"
#include "NodeCallback.h"
#include "spotify/StaticCallbackSetter.h"
#include "spotify/PlaylistContainer.h"
#include "spotify/Playlist.h"
#include "spotify/Track.h"
#include "spotify/Player.h"
#include "spotify/Album.h"

extern "C" {
#include "audio/audio.h"
}

using namespace v8;

SpotifyService* spotifyService;
Player* player;
extern PlaylistContainer* playlistContainer; //defined in SessionCallbacks.cc
extern audio_fifo_t g_audiofifo;

Handle<Value> login(const Arguments& args) {
  HandleScope scope;
  String::Utf8Value v8User(args[0]->ToString());
  String::Utf8Value v8Password(args[1]->ToString());
  bool rememberMe = args[2]->ToBoolean()->Value();
  bool withRemembered = args[3]->ToBoolean()->Value();
  std::string user(*v8User);
  std::string password(*v8Password);
  spotifyService->login(user, password, rememberMe, withRemembered);
  return scope.Close(Undefined());
}

Handle<Value> logout(const Arguments& args) {
  HandleScope scope;
  //TODO: spotifyservice method
  auto callback = [] () { spotifyService->logout(); };
  spotifyService->executeSpotifyAPIcall(callback);
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
    if(nodeCallback->object == 0 || nodeCallback->object->getV8Object().IsEmpty())
      (*fun)->Call(Context::GetCurrent()->Global(), argc, argv);
    else
      (*fun)->Call(nodeCallback->object->getV8Object(), argc, argv);
  }
  scope.Close(Undefined());
}

Handle<Value> rememberedUser(const Arguments& args) {
  HandleScope scope;
  if(spotifyService->rememberedUser != 0) {
    return scope.Close(String::New(spotifyService->rememberedUser));
  } else {
    return scope.Close(Undefined());
  }
}

void init(Handle<Object> target) {
  //google::InitGoogleLogging("node-spotify");
  //LOG(INFO) << "Initializing node.js module";
  Playlist::init(target);
  Track::init(target);
  Artist::init(target);
  Player::init(target);
  Album::init(target);
  StaticCallbackSetter<Playlist>::init(target, "playlists");
  audio_init(&g_audiofifo);
  spotifyService = new SpotifyService();
  player = new Player();
  target->Set(String::NewSymbol("login"),
              FunctionTemplate::New(login)->GetFunction());
  target->Set(String::NewSymbol("logout"),
              FunctionTemplate::New(logout)->GetFunction());
  target->Set(String::NewSymbol("getPlaylists"),
              FunctionTemplate::New(getPlaylists)->GetFunction());
  target->Set(String::NewSymbol("ready"),
              FunctionTemplate::New(ready)->GetFunction());
  target->Set(String::NewSymbol("player"), player->getV8Object());
  target->Set(String::NewSymbol("rememberedUser"),
              FunctionTemplate::New(rememberedUser)->GetFunction());
   
  //Initialize waiting for callbacks from the spotify thread
  uv_async_init(uv_default_loop(), &spotifyService->callNodeThread, resolveCallback);
}
NODE_MODULE(spotify, init)
