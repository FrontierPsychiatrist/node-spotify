#ifndef _V8_CALLABLE_H
#define _V8_CALLABLE_H

#include <string>
#include <map>

#include <v8.h>
#include <uv.h>

/**
 * A base class for objects that want to expose Javascript callbacks to the node thread
 **/
class V8Callable {
	public:
		V8Callable(uv_async_t* _asyncHandle) : asyncHandle(_asyncHandle) {};

		/**
		 *	Saves a V8 callback with a name.
		**/
		void on(std::string name, v8::Persistent<v8::Function> callback);

		/**
	 	 *	Calls a V8 callback by notifying the nodeJS thread.
		 *
		 *	Mainly used from within libspotify callbacks.
		**/
		void call(std::string name);
	private:
		uv_async_t* asyncHandle;
		std::map<std::string, v8::Persistent<v8::Function> > callbacks;
};

#endif
