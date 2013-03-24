#ifndef _STATIC_PLAYLIST_H
#define _STATIC_PLAYLIST_H

#include <node.h>
#include "Playlist.h"


template <class T>
class StaticCallbackSetter : public node::ObjectWrap {
  public:
    static v8::Handle<v8::Value> on(const v8::Arguments& args) {
      v8::HandleScope scope;
      return scope.Close(v8::Undefined());
    };

    static void init(v8::Handle<v8::Object> target, const char* name) {
        HandleScope scope;
        Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
        constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
        target->Set(v8::String::NewSymbol(name), constructorTemplate->GetFunction()->NewInstance());
        scope.Close(v8::Undefined());
    };
};

#endif
