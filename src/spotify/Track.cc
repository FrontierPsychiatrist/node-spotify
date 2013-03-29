#include "Track.h"

Handle<Value> Track::getName(Local<String> property, const AccessorInfo& info) {
  Track* track = node::ObjectWrap::Unwrap<Track>(info.Holder());
  return String::New(track->name.c_str());
}

void Track::init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("Track"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  constructorTemplate->InstanceTemplate()->SetAccessor(String::New("name"), getName, emptySetter);

  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
}
