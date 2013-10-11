#include "NodeSearch.h"
#include "NodeTrack.h"

Handle<Value> NodeSearch::didYouMean(const Arguments& args) {
    HandleScope scope;
    NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
    return scope.Close(String::New(nodeSearch->search->didYouMeanText.c_str()));
}

Handle<Value> NodeSearch::getLink(Local<String> property, const AccessorInfo& info) {
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return String::New(nodeSearch->search->link.c_str());
}

Handle<Value> NodeSearch::getTracks(const Arguments& args) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  std::vector<std::shared_ptr<Track>> tracks = nodeSearch->search->getTracks();
  Local<Array> outArray = Array::New(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    outArray->Set(Number::New(i), nodeTrack->getV8Object());
  }
  return scope.Close(outArray);
}

void NodeSearch::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Search");
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("link"), getLink, emptySetter);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "didYouMean", didYouMean);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getTracks", getTracks);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}