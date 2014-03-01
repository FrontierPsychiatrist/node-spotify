/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#ifndef _NODE_WRAPPED_WITH_CALLBACKS_H
#define _NODE_WRAPPED_WITH_CALLBACKS_H

#include "NodeWrapped.h"
#include "V8Callable.h"

#include <v8.h>
#include <node.h>
#include <string>
#include <initializer_list>

#include "../../Application.h"

extern Application* application;

/**
 * This base class is for javascript objects that should provide callbacks.
 * The callbacks can be set/unset from javascript via on(name, function) / off(name)
 *
 * The callbacks can be called from C++ via the call(name) or call(name, args) method.
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

  void call(std::string name, std::initializer_list<v8::Handle<v8::Value>> args) {
    v8::Handle<v8::Function> callback = getCallback(name);
    if(!callback.IsEmpty() && callback->IsCallable()) {
      unsigned int argc = args.size();
      v8::Handle<v8::Value>* argv = const_cast<v8::Handle<v8::Value>*>(args.begin());
      callback->Call(v8::Context::GetCurrent()->Global(), argc, argv);
    }
  }

  /**
   * Call a Javascript callback by name.
   * If no callback is found, nothing happens.
   **/
  void call(std::string name)  {
    call(name, {v8::Undefined(), this->getV8Object()});
  }
protected:
  /**
   *  Get a callback function by name.
   *  Can be overwritten by extending classes to provide support for static callbacks.
   **/
  virtual v8::Handle<v8::Function> getCallback(std::string name) {
    auto iterator = callbacks.find(name);
    v8::Handle<v8::Function> callback;
    if( iterator != callbacks.end() ) {
      callback = iterator->second;
    }
    return callback;
  }
private:
  std::map<std::string, v8::Persistent<v8::Function>> callbacks;  
};

#endif