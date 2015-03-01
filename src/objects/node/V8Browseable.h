#ifndef _V8_BROWSEABLE_H
#define _V8_BROWSEABLE_H

#include "NodeWrapped.h"

#include <nan.h>
#include <memory>

template<class T>
class V8Browseable : public NodeWrapped<T> {
public:
  void callBrowseComplete() {
    NanScope();
    v8::Handle<v8::Value> argv[2] = {NanUndefined(), NanNew(NanObjectWrapHandle(this))};
    browseCompleteCallback->Call(2, argv);
  }
protected:
  void makePersistent() {
    NanAssignPersistent(persistentHandle, NanObjectWrapHandle(this));
  }
  std::unique_ptr<NanCallback> browseCompleteCallback;
private:
  v8::Persistent<v8::Object> persistentHandle;
};

#endif
