#include "NodeAlbum.h"
#include "NodeTrack.h"
#include "NodeArtist.h"
#include "../spotify/Track.h"

NodeAlbum::NodeAlbum(std::unique_ptr<Album> _album) : album(std::move(_album)) {
  album->nodeObject = this;
};

NodeAlbum::~NodeAlbum() {
  if(album->nodeObject == this) {
    album->nodeObject = nullptr;
  }
}

NAN_GETTER(NodeAlbum::getName) {
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  NanReturnValue(Nan::New<String>(nodeAlbum->album->name().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeAlbum::getLink) {
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  NanReturnValue(Nan::New<String>(nodeAlbum->album->link().c_str()).ToLocalChecked());
}

NAN_METHOD(NodeAlbum::getCoverBase64) {
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  NanReturnValue(Nan::New<String>(nodeAlbum->album->coverBase64().c_str()).ToLocalChecked());
}

NAN_METHOD(NodeAlbum::browse) {
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  if(nodeAlbum->album->albumBrowse == nullptr) {
    nodeAlbum->makePersistent();
    nodeAlbum->browseCompleteCallback.SetFunction(args[0].As<Function>());

    //Mutate the V8 object.
    Handle<Object> nodeAlbumV8 = NanObjectWrapHandle(nodeAlbum);
    nodeAlbumV8->SetAccessor(Nan::New<String>("tracks").ToLocalChecked(), getTracks);
    nodeAlbumV8->SetAccessor(Nan::New<String>("review").ToLocalChecked(), getReview);
    nodeAlbumV8->SetAccessor(Nan::New<String>("copyrights").ToLocalChecked(), getCopyrights);
    nodeAlbumV8->SetAccessor(Nan::New<String>("artist").ToLocalChecked(), getArtist);

    nodeAlbum->album->browse();
  } else {
    nodeAlbum->callBrowseComplete();
  }
  NanReturnUndefined();
}

NAN_GETTER(NodeAlbum::getTracks) {
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  std::vector<std::shared_ptr<Track>> tracks = nodeAlbum->album->tracks();
  Handle<Array> nodeTracks = NanNew<Array>(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    nodeTracks->Set(Nan::New<Number>(i), nodeTrack->createInstance());
  }
  NanReturnValue(nodeTracks);
}

NAN_GETTER(NodeAlbum::getReview) {
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  NanReturnValue(Nan::New<String>(nodeAlbum->album->review().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeAlbum::getCopyrights) {
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  std::vector<std::string> copyrights = nodeAlbum->album->copyrights();
  Handle<Array> nodeCopyrights = NanNew<Array>(copyrights.size());
  for(int i = 0; i < (int)copyrights.size(); i++) {
    nodeCopyrights->Set(Nan::New<Number>(i), Nan::New<String>(copyrights[i].c_str()).ToLocalChecked());
  }
  NanReturnValue(nodeCopyrights);
}

NAN_GETTER(NodeAlbum::getArtist) {
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  NodeArtist* nodeArtist = new NodeArtist(nodeAlbum->album->artist());
  NanReturnValue(nodeArtist->createInstance());
}

NAN_GETTER(NodeAlbum::isLoaded) {
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  NanReturnValue(NanNew<Boolean>(nodeAlbum->album->isLoaded()));
}

void NodeAlbum::init() {
  NanScope();
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Album");
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("name").ToLocalChecked(), getName);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("link").ToLocalChecked(), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("isLoaded").ToLocalChecked(), isLoaded);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getCoverBase64", getCoverBase64);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "browse", browse);
  NanAssignPersistent(NodeAlbum::constructorTemplate, constructorTemplate);
}
