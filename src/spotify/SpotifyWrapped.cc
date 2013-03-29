#include "SpotifyWrapped.h"
#include "../NodeCallback.h"
#include <iostream>
std::map<std::string, v8::Persistent<v8::Function> > SpotifyWrapped::staticCallbacks;

v8::Handle<v8::Object> SpotifyWrapped::getV8Object() {
  v8::HandleScope scope;
  //check if the handle from ObjectWrap has been initialized and if not wrap the object in a new JS instance
  if(handle_.IsEmpty()) {
    v8::Persistent<v8::Object> o = v8::Persistent<v8::Object>::New(getConstructor()->NewInstance());
    this->Wrap(o);
  }
  return scope.Close(v8::Local<v8::Object>::New(handle_));
}

void SpotifyWrapped::emptySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {
  //An empty setter method for unchangeable properties
}

void SpotifyWrapped::on(std::string name, v8::Persistent<v8::Function> callback) {
  callbacks[name] = callback;
}

void SpotifyWrapped::call(std::string name) {
  std::map< std::string, v8::Persistent<v8::Function> >::iterator it;
  it = callbacks.find(name);

  v8::Persistent<v8::Function>* fun = 0;

  //Check if a callback for the given name was found in this object
  if(it != callbacks.end()) {
    //Get the adress of the callback function and send it to the node thread
    //This needs to be the adress from the map element, otherwise we would pass the adress of a local and it fails on the node side.
    fun = &it->second;
  } else {
    //search static callbacks
    it = staticCallbacks.find(name);
    if(it != staticCallbacks.end()) {
      fun = &it->second;
    }
  }
  
  if(fun != 0) {
    NodeCallback* nodeCallback = new NodeCallback();
    nodeCallback->object = this;
    nodeCallback->function = fun;
    asyncHandle->data  = (void*)nodeCallback;
    uv_async_send(asyncHandle);
  }
}
