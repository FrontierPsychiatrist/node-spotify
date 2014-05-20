/**
The MIT License (MIT)

Copyright (c) <2014> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#include "NodeTrackExtended.h"
#include "NodeUser.h"

//Since NodeWrapped uses a templating technique to assign the static constructor to each childclass we need to improvise here.
Persistent<Function> NodeTrackExtended::constructor;

NodeTrackExtended::NodeTrackExtended(std::shared_ptr<TrackExtended> _trackExtended) : NodeTrack(_trackExtended), trackExtended(_trackExtended) {
}

/**
  We need rewrite this method because we need to use our own constructor, not the one from NodeTrack.
**/
Handle<Object> NodeTrackExtended::getV8Object() {
  if(handle_.IsEmpty()) {
    Local<Object> o = Local<Object>::New(constructor->NewInstance());
    this->Wrap(o);
  }
  return handle_;
}

/**
  Same for this... need to rewrite so NodeTrackExtended::constructor is used.
**/
Handle<Function> NodeTrackExtended::getConstructor() {
  return constructor;
}

Handle<Value> NodeTrackExtended::getCreator(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeTrackExtended* nodeTrackExtended = node::ObjectWrap::Unwrap<NodeTrackExtended>(info.Holder());
  std::shared_ptr<User> creator = nodeTrackExtended->trackExtended->creator();
  Handle<Value> nodeCreator = Undefined();
  if(creator.use_count() > 0) {
    NodeUser* nodeUser = new NodeUser(creator);
    nodeCreator = nodeUser->getV8Object();
  }
  return scope.Close(nodeCreator);
}

Handle<Value> NodeTrackExtended::getSeen(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeTrackExtended* nodeTrackExtended = node::ObjectWrap::Unwrap<NodeTrackExtended>(info.Holder());
  return scope.Close(Boolean::New(nodeTrackExtended->trackExtended->seen()));
}

void NodeTrackExtended::setSeen(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  NodeTrackExtended* nodeTrackExtended = node::ObjectWrap::Unwrap<NodeTrackExtended>(info.Holder());
  nodeTrackExtended->trackExtended->seen(value->ToBoolean()->Value());
  scope.Close(Undefined());
}

Handle<Value> NodeTrackExtended::getCreateTime(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeTrackExtended* nodeTrackExtended = node::ObjectWrap::Unwrap<NodeTrackExtended>(info.Holder());
  return scope.Close(Date::New(nodeTrackExtended->trackExtended->createTime() * 1000));
}

Handle<Value> NodeTrackExtended::getMessage(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeTrackExtended* nodeTrackExtended = node::ObjectWrap::Unwrap<NodeTrackExtended>(info.Holder());
  return scope.Close(String::New(nodeTrackExtended->trackExtended->message().c_str()));
}

void NodeTrackExtended::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("TrackExtended");
  setMethods(constructorTemplate);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("creator"), getCreator);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("seen"), getSeen, setSeen);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("createTime"), getCreateTime);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("message"), getMessage);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}