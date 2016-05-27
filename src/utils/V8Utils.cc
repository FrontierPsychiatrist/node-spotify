#include "V8Utils.h"

using namespace v8;

/**
  Get a field from an object as a persistent function handle. Empty handle if the key does not exist.
**/
Nan::Callback V8Utils::getFunctionFromObject(Handle<Object> callbacks, Handle<String> key) {
  NanCallback callback;
  if(callbacks->Has(key)) {
    callback.SetFunction(callbacks->Get(key).As<Function>());
  }
  return callback;
}