#ifndef _NODE_WRAPPED_H
#define _NODE_WRAPPED_H

#include <string>
#include <map>

#include <uv.h>
#include <node.h>
#include <nan.h>

/**
 * A class used as a base class for wrapping objects to node objects.
 **/
template <class T>
class NodeWrapped : public node::ObjectWrap {
public:
  ~NodeWrapped() {}
  
  virtual v8::Handle<v8::Object> createInstance() {
    v8::Local<v8::Object> object = getConstructor()->NewInstance();
    this->Wrap(object);
    return object;
  }

  static v8::Handle<v8::Function> getConstructor() {
    return NanNew(constructorTemplate)->GetFunction();
  }
protected:
  static v8::Persistent<v8::FunctionTemplate> constructorTemplate;

  /**
   * Basic init method for a wrapped node object.
   */
  static v8::Handle<v8::FunctionTemplate> init(const char* className) {
    NanEscapableScope();
    v8::Local<v8::FunctionTemplate> constructorTemplate = NanNew<v8::FunctionTemplate>();
    constructorTemplate->SetClassName(NanNew<v8::String>(className));
    constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    return NanEscapeScope(constructorTemplate);
  }
};

//The constructor template must be static per template instance not for all NodeWrapped subclasses.
template <class T> v8::Persistent<v8::FunctionTemplate> NodeWrapped<T>::constructorTemplate;
#endif
