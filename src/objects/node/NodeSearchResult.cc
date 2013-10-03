#include "NodeSearchResult.h"
#include "NodeTrack.h"

Handle<Value> NodeSearchResult::didYouMean(const Arguments& args) {
    HandleScope scope;
    NodeSearchResult* nodeSearchResult = node::ObjectWrap::Unwrap<NodeSearchResult>(args.This());
    return scope.Close(String::New(nodeSearchResult->searchResult->didYouMeanText));
}

Handle<Value> NodeSearchResult::getTracks(const Arguments& args) {
  HandleScope scope;
  NodeSearchResult* nodeSearchResult = node::ObjectWrap::Unwrap<NodeSearchResult>(args.This());
  std::vector<std::shared_ptr<Track>> tracks = nodeSearchResult->searchResult->getTracks();
  Local<Array> outArray = Array::New(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    outArray->Set(Number::New(i), nodeTrack->getV8Object());
  }
  return scope.Close(outArray);
}

void NodeSearchResult::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("SearchResult");
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "didYouMean", didYouMean);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getTracks", getTracks);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}