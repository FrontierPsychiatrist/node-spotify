#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <node.h>
#include <string>
#include <libspotify/api.h>

#include "SpotifyWrapped.h"

using namespace v8;

class Playlist : public SpotifyWrapped {
	public:
		Playlist(sp_playlist* _playlist, uv_async_t* _handle) : SpotifyWrapped(_handle), playlist(_playlist) {};
		std::string name;

		//Method visible to nodeJS
		static void setName(Local<String> property, Local<Value> value, const AccessorInfo& info);
		static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
		static Handle<Value> onNameChange(const Arguments& args);

		static void init(Handle<Object> target);
	private:
		Persistent<Function> nameChangeCallback;
		sp_playlist* playlist;
};

#endif
