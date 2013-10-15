#ifndef _NODE_WRAPPED_WITH_CALLBACKS_H
#define _NODE_WRAPPED_WITH_CALLBACKS_H

#include "NodeWrapped.h"
#include "V8Callable.h"

#include <v8.h>
#include <node.h>
#include <string>

#include "../../NodeCallback.h"
#include "../../Application.h"

extern Application* application;

/**
 * This base class is for javascript objects that should provide callbacks.
 * The callbacks can be set/unset from javascript via on(name, function) / off(name)
 * There is also the possibility to attach static callbacks, i.e. callbacks that are
 * the same for all instances of a subclass.
 *
 * The callbacks can be called from C++ via the call(name) method. First instance callbacks
 * then static callbacks will be searched for a callback function. To assure callbacks are called
 * from within the node.js thread libuv is used to send an event to the main loop.
 * The function to handle these events is in node-spotify.cc
 **/
template<class T>
class NodeWrappedWithCallbacks : public NodeWrapped<T>, public V8Callable {
template <class S> friend class StaticCallbackSetter;
public:
  /**
   * To set a callback from within C++.
  **/
  void on(std::string name, v8::Handle<v8::Function> callback) {
    v8::HandleScope scope;
    this->callbacks[name] = v8::Persistent<v8::Function>::New(callback);
    scope.Close(v8::Undefined());
  }

  /**
   * Save a Javascript callback under a certain name.
   * This method will be called from Javascript.
   **/
  static v8::Handle<v8::Value> on(const v8::Arguments& args) {
    v8::HandleScope scope;
    T* object = node::ObjectWrap::Unwrap<T>(args.This());
    v8::String::Utf8Value callbackName(args[0]->ToString());
    v8::Handle<v8::Function> fun = v8::Handle<v8::Function>::Cast(args[1]);
    object->callbacks[*callbackName] = v8::Persistent<v8::Function>::New(fun);
    return scope.Close(v8::Undefined());
  }

  /**
  * Deletes all callbacks that are saved under a name.
  **/
  static v8::Handle<v8::Value> off(const v8::Arguments& args) {
    v8::HandleScope scope;
    T* object = node::ObjectWrap::Unwrap<T>(args.This());
    v8::String::Utf8Value callbackName(args[0]->ToString());
    int deleted = object->callbacks.erase(*callbackName);
    return scope.Close(v8::Integer::New(deleted));
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
      //Trigger the nodeJS eventloop. See node-spotify.cc for the recieving function.
      NodeCallback* nodeCallback = new NodeCallback();
      nodeCallback->object = this;
      nodeCallback->function = fun;
      application->asyncHandle.data  = (void*)nodeCallback;
      uv_async_send(&application->asyncHandle);
    }
  }
protected:
  static v8::Handle<v8::FunctionTemplate> init(const char* className) {
    v8::Handle<v8::FunctionTemplate> constructorTemplate = NodeWrapped<T>::init(className);
    NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
    NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "off", off);
    return constructorTemplate;
  }
private:
  std::map<std::string, v8::Persistent<v8::Function>> callbacks;
  static std::map<std::string, v8::Persistent<v8::Function>> staticCallbacks;
};

//This field should be static per template, not for all NodeWrappedWithCallbacks subclasses.
template <class T> std::map<std::string, v8::Persistent<v8::Function>> NodeWrappedWithCallbacks<T>::staticCallbacks;

#endif