#ifndef _STATIC_CALLBACK_SETTER_H
#define _STATIC_CALLBACK_SETTER_H

#include <node.h>

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