#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <node.h>
#include <string>
#include <libspotify/api.h>

using namespace v8;

class Playlist : public node::ObjectWrap {
	public:
		Playlist(sp_playlist* _playlist) : playlist(_playlist) {};
		Handle<Object> getHandle();
		void nameChange();
		static void setName(Local<String> property, Local<Value> value, const AccessorInfo& info);
		static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
		static Handle<Value> onNameChange(const Arguments& args);
		static void init(Handle<Object> target);
		std::string name;
	private:
		Persistent<Function> nameChangeCallback;
		sp_playlist* playlist;
		static Persistent<Function> constructor;
};

#endif
