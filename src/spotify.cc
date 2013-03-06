#include <node.h>
#include <v8.h>

#include "SpotifyService.h"

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

Handle<Value> userName(const Arguments& args) {
	HandleScope scope;
	return scope.Close(Undefined());
}

Handle<Value> playlistsLoaded(const Arguments& args) {
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
		nPlaylists->Set(Number::New(i), playlists[i]->getHandle());
	}
	//Handle<Value> p = playlists.front()->getHandle();
	return scope.Close(nPlaylists);
}

void resolveCallback(uv_async_t* handle, int status) {
	CallbackBase* callback = (CallbackBase*)(handle->data);
	callback->call();
}

void init(Handle<Object> target) {
	Playlist::init(target);
	target->Set(String::NewSymbol("login"),
			        FunctionTemplate::New(login)->GetFunction());
	target->Set(String::NewSymbol("logout"),
			        FunctionTemplate::New(logout)->GetFunction());
	target->Set(String::NewSymbol("getPlaylists"),
			        FunctionTemplate::New(getPlaylists)->GetFunction());
	target->Set(String::NewSymbol("playlistsLoaded"),
			        FunctionTemplate::New(playlistsLoaded)->GetFunction());
	spotifyService = new SpotifyService();
	//Initialize waiting for callbacks from the spotify thread
	uv_async_init(uv_default_loop(), &spotifyService->callNodeThread, resolveCallback);
}
NODE_MODULE(spotify, init)
