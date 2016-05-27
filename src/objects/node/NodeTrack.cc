#include "NodeTrack.h"
#include "NodeArtist.h"
#include "NodeAlbum.h"

NodeTrack::NodeTrack(std::shared_ptr<Track> _track) : track(_track) {

};

NodeTrack::~NodeTrack() {

}

NAN_GETTER(NodeTrack::getName) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.This());
  NanReturnValue(Nan::New<String>(nodeTrack->track->name().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeTrack::getLink) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.This());
  NanReturnValue(Nan::New<String>(nodeTrack->track->link().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeTrack::getDuration) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.This());
  NanReturnValue(NanNew<Integer>(nodeTrack->track->duration()/1000));
}

NAN_GETTER(NodeTrack::getAvailability) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.This());
  NanReturnValue(NanNew<Integer>(nodeTrack->track->getAvailability()));
}

NAN_GETTER(NodeTrack::getPopularity) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.This());
  NanReturnValue(NanNew<Integer>(nodeTrack->track->popularity()));
}

NAN_GETTER(NodeTrack::getArtists) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.This());
  Local<Array> jsArtists = NanNew<Array>(nodeTrack->track->artists().size());
  for(int i = 0; i < (int)nodeTrack->track->artists().size(); i++) {
    if(nodeTrack->track->artists()[i]) {
      NodeArtist* nodeArtist = new NodeArtist(std::move(nodeTrack->track->artists()[i]));
      jsArtists->Set(Nan::New<Number>(i), nodeArtist->createInstance());
    } else {
      jsArtists->Set(Nan::New<Number>(i), Nan::Undefined());
    }
  }
  NanReturnValue(jsArtists);
}

NAN_GETTER(NodeTrack::getAlbum) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.This());
  if(nodeTrack->track->album()) {
    NodeAlbum* nodeAlbum = new NodeAlbum(nodeTrack->track->album());
    NanReturnValue(nodeAlbum->createInstance());
  } else {
    NanReturnUndefined();
  }
}

NAN_GETTER(NodeTrack::getStarred) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.This());
  NanReturnValue(NanNew<Boolean>(nodeTrack->track->starred()));
}

NAN_SETTER(NodeTrack::setStarred) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.This());
  nodeTrack->track->setStarred(value->ToBoolean()->Value());
}

NAN_GETTER(NodeTrack::isLoaded) {
  NodeTrack* nodeTrack = node::ObjectWrap::Unwrap<NodeTrack>(info.This());
  NanReturnValue(NanNew<Boolean>(nodeTrack->track->isLoaded()));
}

Handle<FunctionTemplate> NodeTrack::init() {
  Nan::EscapableHandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Track");
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("name").ToLocalChecked(), getName);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("link").ToLocalChecked(), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("duration").ToLocalChecked(), getDuration);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("artists").ToLocalChecked(), getArtists);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("album").ToLocalChecked(), getAlbum);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("starred").ToLocalChecked(), getStarred, setStarred);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("popularity").ToLocalChecked(), getPopularity);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("isLoaded").ToLocalChecked(), isLoaded);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("availability").ToLocalChecked(), getAvailability);
  NanAssignPersistent(NodeTrack::constructorTemplate, constructorTemplate);
  return scope.Escape(constructorTemplate);
}
