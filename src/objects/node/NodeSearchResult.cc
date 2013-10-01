#include "NodeSearchResult.h"

Handle<Value> NodeSearchResult::didYouMean(const Arguments& args) {
    HandleScope scope;
    NodeSearchResult* nodeSearchResult = node::ObjectWrap::Unwrap<NodeSearchResult>(args.This());
    return scope.Close(String::New(nodeSearchResult->searchResult->didYouMeanText));
}

void NodeSearchResult::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("SearchResult");
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "didYouMean", didYouMean);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}