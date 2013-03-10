#include <node.h>
#include <v8.h>

#include "SpotifyService/SpotifyService.h"
#include "NodeCallback.h"

using namespace v8;

SpotifyService* spotifyService;

Handle<Value> login(const Arguments& args) {
	HandleScope scope;
	std::string user("USER");
	std::string password("PASSWORD");
	spotifyService->login(user, password);
	return scope.Close(Undefined());
}

Handle<Value> logout(const Arguments& args) {
	HandleScope scope;
	Callback<SpotifyService>* logoutCallback = new Callback<SpotifyService>(spotifyService, &SpotifyService::logout);
	spotifyService->executeSpotifyAPIcall(logoutCallback);
	return Undefined();
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
	std::vector<Playlist*> playlists = spotifyService->getPlaylistContainer()->getPlaylists();
	Local<Array> nPlaylists = Array::New(playlists.size());
	for(int i = 0; i < (int)playlists.size(); i++) {
		nPlaylists->Set(Number::New(i), *(playlists[i]->getV8Object()) );
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
    if(nodeCallback->object == NULL)
		  (*fun)->Call(Context::GetCurrent()->Global(), argc, argv);
    else
		  (*fun)->Call(*nodeCallback->object, argc, argv);
	}
	scope.Close(Undefined());
}

void init(Handle<Object> target) {
	Playlist::init(target);
	target->Set(String::NewSymbol("login"),
			        FunctionTemplate::New(login)->GetFunction());
	target->Set(String::NewSymbol("logout"),
			        FunctionTemplate::New(logout)->GetFunction());
	target->Set(String::NewSymbol("getPlaylists"),
			        FunctionTemplate::New(getPlaylists)->GetFunction());
	target->Set(String::NewSymbol("ready"),
			        FunctionTemplate::New(ready)->GetFunction());
	spotifyService = new SpotifyService();
	//Initialize waiting for callbacks from the spotify thread
	uv_async_init(uv_default_loop(), &spotifyService->callNodeThread, resolveCallback);
}
NODE_MODULE(spotify, init)
