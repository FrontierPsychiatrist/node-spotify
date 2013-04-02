#include "Track.h"

Handle<Value> Track::getName(Local<String> property, const AccessorInfo& info) {
  Track* track = node::ObjectWrap::Unwrap<Track>(info.Holder());
  return String::New(track->name.c_str());
}

Handle<Value> Track::getArtists(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  Track* track = node::ObjectWrap::Unwrap<Track>(info.Holder());
  Local<Array> jsArtists = Array::New(track->artists.size());
  for(int i = 0; i < (int)track->artists.size(); i++) {
    jsArtists->Set(Number::New(i), track->artists[i]->getV8Object() );
  }
  return scope.Close(jsArtists); 
}

void Track::init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("Track"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("artists"), getArtists, emptySetter);

  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
}
