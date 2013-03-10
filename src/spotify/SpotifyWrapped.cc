#include "SpotifyWrapped.h"

/**
 * The extending objects must initialize the ctor in their init method.
 **/
v8::Persistent<v8::Function> SpotifyWrapped::constructor;

v8::Handle<v8::Object>* SpotifyWrapped::getV8Object() {
	//v8::HandleScope scope;
	//check if the handle from ObjectWrap has been initialized and if not wrap the object in a new JS instance
	if(handle_.IsEmpty()) {
		v8::Local<v8::Object> o = constructor->NewInstance();
		this->Wrap(o);
	}
	return &handle_;//scope.Close(handle_);
}

void SpotifyWrapped::call(std::string name) {
  V8Callable::call(name, this->getV8Object());
}
