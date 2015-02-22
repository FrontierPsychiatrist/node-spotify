#ifndef _V8_BROWSEABLE_H
#define _V8_BROWSEABLE_H

#include "V8Wrapped.h"

#include <nan.h>

class V8Browseable : public virtual V8Wrapped {
public:
  void callBrowseComplete() {
    NanScope();
    v8::Handle<v8::Value> argv[2] = {NanUndefined(), NanNew(this->getV8Object())};
    browseCompleteCallback->Call(2, argv);
  }
protected:
  void makePersistent() {
    NanAssignPersistent(persistentHandle, this->getV8Object());
    //FIXME persistentHandle = v8::Persistent<v8::Object>::New(this->getV8Object());
  }
  std::unique_ptr<NanCallback> browseCompleteCallback;
private:
  v8::Persistent<v8::Object> persistentHandle;
};

#endif
