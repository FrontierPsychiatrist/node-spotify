#ifndef _SPOTIFY_WRAPPED_H
#define _SPOTIFY_WRAPPED_H

#include <node.h>
#include "V8Callable.h"

/**
 * A class used as a base class for wrapping libspotify types to V8 objects.
**/
class SpotifyWrapped : public V8Callable, public node::ObjectWrap {
	public:
		SpotifyWrapped(uv_async_t* _handle) : V8Callable(_handle) {}; 
		v8::Handle<v8::Object>* getV8Object();
    void call(std::string name);
	protected:
		static v8::Persistent<v8::Function> constructor;	
};

#endif
