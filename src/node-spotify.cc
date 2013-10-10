#include <node.h>
#include <v8.h>

#include "Application.h"
#include "SpotifyService/SpotifyService.h"
#include "SpotifyService/SearchCallbacks.h"
#include "NodeCallback.h"
#include "events.h"

#include "objects/node/StaticCallbackSetter.h"
#include "objects/spotify/PlaylistContainer.h"
#include "objects/node/NodePlaylist.h"
#include "objects/node/NodeTrack.h"
#include "objects/node/NodePlayer.h"
#include "objects/node/NodeAlbum.h"
#include "objects/node/NodeArtist.h"
#include "objects/node/NodeSearchResult.h"

extern "C" {
  #include "audio/audio.h"
}

using namespace v8;

Application* application;

Handle<Value> login(const Arguments& args) {
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

Handle<Value> logout(const Arguments& args) {
  HandleScope scope;
  auto callback = [] () { application->spotifyService->logout(); };
  application->spotifyService->executeSpotifyAPIcall(callback);
  return scope.Close(Undefined());
}

Handle<Value> ready(const Arguments& args) {
  HandleScope scope;
  Handle<Function> fun = Handle<Function>::Cast(args[0]);
  Persistent<Function> p = Persistent<Function>::New(fun);
  application->loginCallback = p;
  return scope.Close(Undefined());
}

Handle<Value> search(const Arguments& args) {
  HandleScope scope;
  //TODO: lots! of parameters!
  String::Utf8Value searchString(args[0]->ToString());
  const char* searchStringChar = *searchString;
  Persistent<Function> fun = Persistent<Function>::New(Handle<Function>::Cast(args[1]));
  NodeSearchResult* searchResult = new NodeSearchResult();
  searchResult->on(SEARCH_COMPLETE, fun);
  auto search = [=] () {
    sp_search_create(application->session,
                    searchStringChar,
                    0, 20, //track offset + count      
                    0, 10, //album offest + count
                    0, 5, //artist offset + count
                    0, 1, //playlist offset + count
                    SP_SEARCH_STANDARD, //?
                    SearchCallbacks::searchComplete,
                    searchResult
                    );
  };
  application->spotifyService->executeSpotifyAPIcall(search);
  return scope.Close(Undefined());
}

Handle<Value> getPlaylists(const Arguments& args) {
  HandleScope scope;
  std::vector<std::shared_ptr<Playlist>> playlists = application->playlistContainer->getPlaylists();
  Local<Array> nPlaylists = Array::New(playlists.size());
  for(int i = 0; i < (int)playlists.size(); i++) {
    NodePlaylist* nodePlaylist = new NodePlaylist(playlists[i]);
    nPlaylists->Set(Number::New(i), nodePlaylist->getV8Object());
  }
  return scope.Close(nPlaylists);
}

Handle<Value> getStarred(const Arguments& args) {
  HandleScope scope;
  NodePlaylist* starredPlaylist = new NodePlaylist(application->playlistContainer->starredPlaylist);
  return scope.Close(starredPlaylist->getV8Object());
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
  if(application->spotifyService->rememberedUser != 0) {
    return scope.Close(String::New(application->spotifyService->rememberedUser));
  } else {
    return scope.Close(Undefined());
  }
}

void init(Handle<Object> target) {
  NodePlaylist::init();
  NodeTrack::init();
  NodeArtist::init();
  NodePlayer::init();
  NodeAlbum::init();
  NodeSearchResult::init();
  StaticCallbackSetter<NodePlaylist>::init(target, "playlists");
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
  target->Set(String::NewSymbol("search"),
              FunctionTemplate::New(search)->GetFunction());
   
  //Initialize waiting for callbacks from the spotify thread
  uv_async_init(uv_default_loop(), &application->asyncHandle, resolveCallback);
}
NODE_MODULE(spotify, init)
