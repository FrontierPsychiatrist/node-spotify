#include "NodeArtist.h"

Handle<Value> NodeArtist::getName(Local<String> property, const AccessorInfo& info) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());
  return String::New(nodeArtist->artist->name.c_str());
}

void NodeArtist::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Artist");

  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, emptySetter);

  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}