#include "V8Utils.h"

using namespace v8;

/**
  Get a field from an object as a persistent function handle. Empty handle if the key does not exist.
**/
std::unique_ptr<NanCallback> V8Utils::getFunctionFromObject(Handle<Object> callbacks, Handle<String> key) {
  std::unique_ptr<NanCallback> callback;
  if(callbacks->Has(key)) {
    callback = std::unique_ptr<NanCallback>(new NanCallback(callbacks->Get(key).As<Function>()));
  }
  return callback;
}