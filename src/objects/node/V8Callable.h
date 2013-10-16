#ifndef _V8_CALLABLE_H
#define _V8_CALLABLE_H

#include <string>
#include <v8.h>

class V8Callable {
public:
  virtual void call(std::string name) = 0;
  virtual void call(std::string name, v8::Handle<v8::Value> value) = 0;
};

#endif