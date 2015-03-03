#ifndef _V8_UTILS_H
#define _V8_UTILS_H

#include <nan.h>
#include <memory>

class V8Utils {
public:
  static std::unique_ptr<NanCallback> getFunctionFromObject(v8::Handle<v8::Object> callbacks, v8::Handle<v8::String> key);
};

#endif