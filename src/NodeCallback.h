#ifndef _NODECALLBACK_H
#define _NODECALLBACK_H

#include <v8.h>

class V8Wrapped;

struct NodeCallback {
  V8Wrapped* object;
  v8::Persistent<v8::Function>* function;
};

#endif
