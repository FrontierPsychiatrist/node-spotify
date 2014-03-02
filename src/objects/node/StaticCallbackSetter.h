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

#ifndef _STATIC_CALLBACK_SETTER_H
#define _STATIC_CALLBACK_SETTER_H

#include <node.h>
#include <v8.h>

template <class T>
class StaticCallbackSetter : public node::ObjectWrap {
public:
  static v8::Handle<v8::Value> on(const v8::Arguments& args) {
    v8::HandleScope scope;
    v8::String::Utf8Value callbackName(args[0]->ToString());
    v8::Handle<v8::Function> fun = v8::Handle<v8::Function>::Cast(args[1]);
    T::staticCallbacks[*callbackName] = v8::Persistent<v8::Function>::New(fun);
    return scope.Close(v8::Undefined());
  };

  static void init(Handle<Object> target, const char* name) {
    v8::HandleScope scope;
    v8::Local<v8::FunctionTemplate> constructorTemplate = v8::FunctionTemplate::New();
    constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
    target->Set(v8::String::NewSymbol(name), constructorTemplate->GetFunction()->NewInstance());
    scope.Close(constructorTemplate->GetFunction()->NewInstance());
  };
};

#endif