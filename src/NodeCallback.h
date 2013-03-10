#ifndef _NODECALLBACK_H
#define _NODECALLBACK_H

#include <v8.h>

struct NodeCallback {
  v8::Handle<v8::Object>* object;
  v8::Persistent<v8::Function>* function;
};

#endif
