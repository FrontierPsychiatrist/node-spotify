#ifndef _NODE_WRAPPED_H
#define _NODE_WRAPPED_H

#include <string>
#include <map>

#include <uv.h>
#include <node.h>
#include <nan.h>

#include "V8Wrapped.h"

/**
 * A class used as a base class for wrapping objects to node objects.
 **/
template <class T>
class NodeWrapped : public node::ObjectWrap, public virtual V8Wrapped {
public:
  ~NodeWrapped() {}
  /**
   * Get a V8 handle with the Javascript object inside.
   **/
  virtual v8::Handle<v8::Object> getV8Object() {
    return NanObjectWrapHandle(this);
  }

  virtual v8::Handle<v8::Object> createInstance() {
    v8::Local<v8::Object> object = NanNew(constructor)->NewInstance();
    NanSetInternalFieldPointer(object, 0, this);
    return object;
  }

  static v8::Handle<v8::Function> getConstructor() {
    return constructor;
  }
protected:
  static v8::Handle<v8::Function> constructor;

  /**
   * Basic init method for a wrapped node object.
   */
  static v8::Handle<v8::FunctionTemplate> init(const char* className) {
    v8::Local<v8::FunctionTemplate> constructorTemplate = NanNew<v8::FunctionTemplate>();
    constructorTemplate->SetClassName(NanNew<v8::String>(className));
    constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    return constructorTemplate;
  }
};

//The constructor must be static per template instance not fro all NodeWrapped subclasses.
template <class T> v8::Handle<v8::Function> NodeWrapped<T>::constructor;
#endif
