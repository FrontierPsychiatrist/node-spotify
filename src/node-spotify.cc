#include <node.h>
#include <v8.h>

#include "Application.h"
#include "SpotifyService/SpotifyService.h"
#include "SpotifyService/SessionCallbacks.h"

#include "objects/node/StaticCallbackSetter.h"
#include "objects/spotify/PlaylistContainer.h"
#include "objects/node/NodePlaylist.h"
#include "objects/node/NodeTrack.h"
#include "objects/node/NodePlayer.h"
#include "objects/node/NodeAlbum.h"
#include "objects/node/NodeArtist.h"
#include "objects/node/NodeSearch.h"

extern "C" {
  #include "audio/audio.h"
}

using namespace v8;

Application* application;

static Handle<Value> login(const Arguments& args) {
  HandleScope scope;
  String::Utf8Value v8User(args[0]->ToString());
  String::Utf8Value v8Password(args[1]->ToString());
  bool rememberMe = args[2]->ToBoolean()->Value();
  bool withRemembered = args[3]->ToBoolean()->Value();
  std::string user(*v8User);
  std::string password(*v8Password);
  application->spotifyService->login(user, password, rememberMe, withRemembered);
  return scope.Close(Undefined());
}

static Handle<Value> logout(const Arguments& args) {
  HandleScope scope;
  application->spotifyService->logout();
  return scope.Close(Undefined());
}

static Handle<Value> ready(const Arguments& args) {
  HandleScope scope;
  Handle<Function> fun = Handle<Function>::Cast(args[0]);
  Persistent<Function> p = Persistent<Function>::New(fun);
  SessionCallbacks::loginCallback = p;
  return scope.Close(Undefined());
}

static Handle<Value> getPlaylists(const Arguments& args) {
  HandleScope scope;
  std::vector<std::shared_ptr<Playlist>> playlists = application->playlistContainer->getPlaylists();
  Local<Array> nPlaylists = Array::New(playlists.size());
  for(int i = 0; i < (int)playlists.size(); i++) {
    NodePlaylist* nodePlaylist = new NodePlaylist(playlists[i]);
    nPlaylists->Set(Number::New(i), nodePlaylist->getV8Object());
  }
  return scope.Close(nPlaylists);
}

static Handle<Value> getStarred(const Arguments& args) {
  HandleScope scope;
  NodePlaylist* starredPlaylist = new NodePlaylist(application->playlistContainer->starredPlaylist);
  return scope.Close(starredPlaylist->getV8Object());
}

static Handle<Value> rememberedUser(const Arguments& args) {
  HandleScope scope;
  int maxUsernameLength = 256;
  char rememberedUser[maxUsernameLength];
  int usernameLength = sp_session_remembered_user(application->session, rememberedUser, maxUsernameLength);
  if(usernameLength != -1) {
    return scope.Close(String::New(rememberedUser));
  } else {
    return scope.Close(Undefined());
  }
}

static void init(Handle<Object> target) {
  NodePlaylist::init();
  NodeTrack::init();
  NodeArtist::init();
  NodePlayer::init();
  NodeAlbum::init();
  NodeSearch::init(target);
  StaticCallbackSetter<NodePlaylist>::init(target, "playlists");

  SessionCallbacks::init();
  application = new Application();
  application->spotifyService = std::unique_ptr<SpotifyService>(new SpotifyService());
  audio_init(&application->audio_fifo);

  target->Set(String::NewSymbol("login"),
              FunctionTemplate::New(login)->GetFunction());
  target->Set(String::NewSymbol("logout"),
              FunctionTemplate::New(logout)->GetFunction());
  target->Set(String::NewSymbol("getPlaylists"),
              FunctionTemplate::New(getPlaylists)->GetFunction());
  target->Set(String::NewSymbol("getStarred"),
              FunctionTemplate::New(getStarred)->GetFunction());
  target->Set(String::NewSymbol("ready"),
              FunctionTemplate::New(ready)->GetFunction());
  target->Set(String::NewSymbol("player"), NodePlayer::getInstance().getV8Object());
  target->Set(String::NewSymbol("rememberedUser"),
              FunctionTemplate::New(rememberedUser)->GetFunction());
}
NODE_MODULE(spotify, init)
