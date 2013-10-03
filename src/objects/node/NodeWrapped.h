#ifndef _NODE_WRAPPED_H
#define _NODE_WRAPPED_H

#include <string>
#include <map>

#include <uv.h>
#include <node.h>

#include "../../NodeCallback.h"
#include "../../Application.h"
#include "V8Wrapped.h"

extern Application* application;

/**
 * A class used as a base class for wrapping objects to node objects
 * 
 * Provides callback handling
 **/
template <class T>
class NodeWrapped : public node::ObjectWrap, V8Wrapped {
template <class S> friend class StaticCallbackSetter;
public:
  /**
   * Get a V8 handle with the Javascript object inside.
   **/
  v8::Handle<v8::Object> getV8Object() {
    //We cannot open a new HandleScope here, as this gets called in the spotify thread!
    //check if the handle from ObjectWrap has been initialized and if not wrap the object in a new JS instance
    if(handle_.IsEmpty()) {
      v8::Persistent<v8::Object> o = v8::Persistent<v8::Object>::New(constructor->NewInstance());
      this->Wrap(o);
    }
    return handle_;
  }

  /**
   * Save a Javascript callback under a certain name.
   **/
  void on(std::string name, v8::Persistent<v8::Function> callback) {
    callbacks[name] = callback;
  }

  /**
   * Call a Javascript callback by name. The callback will be executed in the nodeJS thread.
   * First, object wide callbacks will be searched, then, class wide callbacks.
   * If no callback is found, nothing happens.
   **/
  void call(std::string name)  {
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
      //Trigger the nodeJS eventloop
      NodeCallback* nodeCallback = new NodeCallback();
      nodeCallback->object = this;
      nodeCallback->function = fun;
      application->asyncHandle.data  = (void*)nodeCallback;
      uv_async_send(&application->asyncHandle);
    }
  };
protected:
  static void emptySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {};
  static v8::Persistent<v8::Function> constructor;
private:
  std::map<std::string, v8::Persistent<v8::Function> > callbacks;
  static std::map<std::string, v8::Persistent<v8::Function> > staticCallbacks;
};

template <class T> std::map<std::string, v8::Persistent<v8::Function> > NodeWrapped<T>::staticCallbacks;
template <class T> v8::Persistent<v8::Function> NodeWrapped<T>::constructor;
#endif
