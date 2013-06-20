#ifndef _V8_WRAPPED
#define _V8_WRAPPED

#include <v8.h>

class V8Wrapped {
  public:
    virtual ~V8Wrapped() {};
    virtual v8::Handle<v8::Object> getV8Object() = 0;
};

#endif