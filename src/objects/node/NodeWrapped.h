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
  ~NodeWrapped() {}
  /**
   * Get a V8 handle with the Javascript object inside.
   **/
  v8::Handle<v8::Object> getV8Object() {
    v8::HandleScope scope;
    //check if the handle from ObjectWrap has been initialized and if not wrap the object in a new JS instance
    if(handle_.IsEmpty()) {
      v8::Local<v8::Object> o = v8::Local<v8::Object>::New(constructor->NewInstance());
      this->Wrap(o);
    }
    return scope.Close(handle_);
  }

  /**
   * To set a callback from within C++
  **/
  void on(std::string name, v8::Handle<v8::Function> callback) {
    v8::HandleScope scope;
    this->callbacks[name] = v8::Persistent<v8::Function>::New(callback);
    scope.Close(v8::Undefined());
  }

  /**
   * Save a Javascript callback under a certain name.
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
  /**
   * Basic init method for a wrapped node object. Provides a callback setter "on" and sets the classname.
   */
  static v8::Handle<v8::FunctionTemplate> init(const char* className) {
    v8::Local<v8::FunctionTemplate> constructorTemplate = v8::FunctionTemplate::New();
    constructorTemplate->SetClassName(v8::String::NewSymbol(className));
    constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
    NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "off", off);
    return constructorTemplate;
  }
private:
  std::map<std::string, v8::Persistent<v8::Function> > callbacks;
  static std::map<std::string, v8::Persistent<v8::Function> > staticCallbacks;
};

template <class T> std::map<std::string, v8::Persistent<v8::Function> > NodeWrapped<T>::staticCallbacks;
template <class T> v8::Persistent<v8::Function> NodeWrapped<T>::constructor;
#endif
