#include "NodeTrackExtended.h"
#include "NodeUser.h"

//Since NodeWrapped uses a templating technique to assign the static constructor to each childclass we need to improvise here.
Persistent<FunctionTemplate> NodeTrackExtended::constructorTemplate;

NodeTrackExtended::NodeTrackExtended(std::shared_ptr<TrackExtended> _trackExtended) : NodeTrack(_trackExtended), trackExtended(_trackExtended) {
}

/**
  We need rewrite this method because we need to use our own constructor, not the one from NodeTrack.
**/
Handle<Object> NodeTrackExtended::createInstance() {
  Local<Object> object = getConstructor()->NewInstance();
  NanSetInternalFieldPointer(object, 0, this);
  return object;
}

/**
  Same for this... we need to rewrite so NodeTrackExtended::constructor is used and not NodeTrack::constructor.
**/
Handle<Function> NodeTrackExtended::getConstructor() {
  return NanNew(constructorTemplate)->GetFunction();
}

NAN_GETTER(NodeTrackExtended::getCreator) {
  NodeTrackExtended* nodeTrackExtended = node::ObjectWrap::Unwrap<NodeTrackExtended>(info.This());
  Handle<Value> nodeCreator = Nan::Undefined();
  if(nodeTrackExtended->trackExtended->creator()) {
    NodeUser* nodeUser = new NodeUser(nodeTrackExtended->trackExtended->creator());
    nodeCreator = nodeUser->createInstance();
  }
  NanReturnValue(nodeCreator);
}

NAN_GETTER(NodeTrackExtended::getSeen) {
  NodeTrackExtended* nodeTrackExtended = node::ObjectWrap::Unwrap<NodeTrackExtended>(info.This());
  NanReturnValue(NanNew<Boolean>(nodeTrackExtended->trackExtended->seen()));
}

NAN_SETTER(NodeTrackExtended::setSeen) {
  NodeTrackExtended* nodeTrackExtended = node::ObjectWrap::Unwrap<NodeTrackExtended>(info.This());
  nodeTrackExtended->trackExtended->seen(value->ToBoolean()->Value());
}

NAN_GETTER(NodeTrackExtended::getCreateTime) {
  NodeTrackExtended* nodeTrackExtended = node::ObjectWrap::Unwrap<NodeTrackExtended>(info.This());
  NanReturnValue(NanNew<Date>(nodeTrackExtended->trackExtended->createTime() * 1000));
}

NAN_GETTER(NodeTrackExtended::getMessage) {
  NodeTrackExtended* nodeTrackExtended = node::ObjectWrap::Unwrap<NodeTrackExtended>(info.This());
  NanReturnValue(Nan::New<String>(nodeTrackExtended->trackExtended->message().c_str()).ToLocalChecked());
}

void NodeTrackExtended::init() {
  NanScope();
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("TrackExtended");
  Handle<FunctionTemplate> nodeTrackTemplate = NodeTrack::init();
  constructorTemplate->Inherit(nodeTrackTemplate);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("creator").ToLocalChecked(), getCreator);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("seen").ToLocalChecked(), getSeen, setSeen);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("createTime").ToLocalChecked(), getCreateTime);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("message").ToLocalChecked(), getMessage);
  NanAssignPersistent(NodeTrackExtended::constructorTemplate, constructorTemplate);
}
