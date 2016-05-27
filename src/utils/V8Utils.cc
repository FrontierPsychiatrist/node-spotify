#include "V8Utils.h"

using namespace v8;

/**
  Get a field from an object as a persistent function handle. Empty handle if the key does not exist.
**/
Handle<Function> V8Utils::getFunctionFromObject(Handle<Object> callbacks, Handle<String> key) {
  Handle<Function> callback;
  if(callbacks->Has(key)) {
    callback = callbacks->Get(key).As<Function>();
  }
  return callback;
}