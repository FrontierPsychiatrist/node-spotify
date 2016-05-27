#ifndef _V8_BROWSEABLE_H
#define _V8_BROWSEABLE_H

#include "NodeWrapped.h"

#include <nan.h>

template<class T>
class V8Browseable : public NodeWrapped<T> {
public:
  void callBrowseComplete() {
    Nan::HandleScope scope;
    v8::Handle<v8::Value> argv[2] = {Nan::Undefined(), Nan::New(NanObjectWrapHandle(this))};
    browseCompleteCallback.Call(2, argv);
  }
protected:
  void makePersistent() {
    NanAssignPersistent(persistentHandle, NanObjectWrapHandle(this));
  }
  Nan::Callback browseCompleteCallback;
private:
  v8::Persistent<v8::Object> persistentHandle;
};

#endif
