#ifndef _V8_CALLABLE_H
#define _V8_CALLABLE_H

#include <string>

class V8Callable {
public:
  virtual void call(std::string name) = 0; 
};

#endif