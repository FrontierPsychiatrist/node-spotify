#include "NodeTrackExtended.h"
#include "NodeUser.h"

//Since NodeWrapped uses a templating technique to assign the static constructor to each childclass we need to improvise here.
Nan::Persistent<FunctionTemplate> NodeTrackExtended::constructorTemplate;

NodeTrackExtended::NodeTrackExtended(std::shared_ptr<TrackExtended> _trackExtended) : NodeTrack(_trackExtended), trackExtended(_trackExtended) {
}

/**
  We need rewrite this method because we need to use our own constructor, not the one from NodeTrack.
**/
Handle<Object> NodeTrackExtended::createInstance() {
  Local<Object> object = getConstructor()->NewInstance();
  Nan::SetInternalFieldPointer(object, 0, this);
  return object;
}

/**
  Same for this... we need to rewrite so NodeTrackExtended::constructor is used and not NodeTrack::constructor.
**/
Handle<Function> NodeTrackExtended::getConstructor() {
  return Nan::New(constructorTemplate)->GetFunction();
}

NAN_GETTER(NodeTrackExtended::getCreator) {
  NodeTrackExtended* nodeTrackExtended = Nan::ObjectWrap::Unwrap<NodeTrackExtended>(info.This());
  Handle<Value> nodeCreator = Nan::Undefined();
  if(nodeTrackExtended->trackExtended->creator()) {
    NodeUser* nodeUser = new NodeUser(nodeTrackExtended->trackExtended->creator());
    nodeCreator = nodeUser->createInstance();
  }
  info.GetReturnValue().Set(nodeCreator);
}

NAN_GETTER(NodeTrackExtended::getSeen) {
  NodeTrackExtended* nodeTrackExtended = Nan::ObjectWrap::Unwrap<NodeTrackExtended>(info.This());
  info.GetReturnValue().Set(Nan::New<Boolean>(nodeTrackExtended->trackExtended->seen()));
}

NAN_SETTER(NodeTrackExtended::setSeen) {
  NodeTrackExtended* nodeTrackExtended = Nan::ObjectWrap::Unwrap<NodeTrackExtended>(info.This());
  nodeTrackExtended->trackExtended->seen(value->ToBoolean()->Value());
}

NAN_GETTER(NodeTrackExtended::getCreateTime) {
  NodeTrackExtended* nodeTrackExtended = Nan::ObjectWrap::Unwrap<NodeTrackExtended>(info.This());
  info.GetReturnValue().Set(Nan::New<Date>(nodeTrackExtended->trackExtended->createTime() * 1000).ToLocalChecked());
}

NAN_GETTER(NodeTrackExtended::getMessage) {
  NodeTrackExtended* nodeTrackExtended = Nan::ObjectWrap::Unwrap<NodeTrackExtended>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodeTrackExtended->trackExtended->message().c_str()).ToLocalChecked());
}

void NodeTrackExtended::init() {
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("TrackExtended");
  Handle<FunctionTemplate> nodeTrackTemplate = NodeTrack::init();
  constructorTemplate->Inherit(nodeTrackTemplate);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("creator").ToLocalChecked(), getCreator);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("seen").ToLocalChecked(), getSeen);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("createTime").ToLocalChecked(), getCreateTime);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("message").ToLocalChecked(), getMessage);
  NodeTrackExtended::constructorTemplate.Reset(constructorTemplate);
}
