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

#ifndef _NODE_WRAPPED_H
#define _NODE_WRAPPED_H

#include <string>
#include <map>

#include <uv.h>
#include <node.h>

#include "V8Wrapped.h"

/**
 * A class used as a base class for wrapping objects to node objects.
 **/
template <class T>
class NodeWrapped : public node::ObjectWrap, public virtual V8Wrapped {
public:
  ~NodeWrapped() {}
  /**
   * Get a V8 handle with the Javascript object inside.
   **/
  virtual v8::Handle<v8::Object> getV8Object() {
    //check if the handle from ObjectWrap has been initialized and if not wrap the object in a new JS instance
    if(handle_.IsEmpty()) {
      v8::Local<v8::Object> o = v8::Local<v8::Object>::New(constructor->NewInstance());
      this->Wrap(o);
    }
    return handle_;
  }
protected:
  /**
   * A setter that does nothing which is useful for defining properties that should be read only
   **/
  static void emptySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {};

  static v8::Persistent<v8::Function> constructor;

  /**
   * Basic init method for a wrapped node object.
   */
  static v8::Handle<v8::FunctionTemplate> init(const char* className) {
    v8::Local<v8::FunctionTemplate> constructorTemplate = v8::FunctionTemplate::New();
    constructorTemplate->SetClassName(v8::String::NewSymbol(className));
    constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    return constructorTemplate;
  }
};

//The constructor must be static per template instance not fro all NodeWrapped subclasses.
template <class T> v8::Persistent<v8::Function> NodeWrapped<T>::constructor;
#endif
