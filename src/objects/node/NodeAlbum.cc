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
  NodeAlbum* nodeAlbum = Nan::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodeAlbum->album->name().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeAlbum::getLink) {
  NodeAlbum* nodeAlbum = Nan::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodeAlbum->album->link().c_str()).ToLocalChecked());
}

NAN_METHOD(NodeAlbum::getCoverLink) {
  sp_image_size imageSize = SP_IMAGE_SIZE_NORMAL;
  if(info.Length() > 0 && info[0]->IsNumber()) {
    imageSize = static_cast<sp_image_size>(info[0]->ToNumber()->IntegerValue());
  }
  NodeAlbum* nodeAlbum = Nan::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodeAlbum->album->coverLink(imageSize).c_str()).ToLocalChecked());
}

NAN_METHOD(NodeAlbum::getCoverBase64) {
  sp_image_size imageSize = SP_IMAGE_SIZE_NORMAL;
  if(info.Length() > 0 && info[0]->IsNumber()) {
    imageSize = static_cast<sp_image_size>(info[0]->ToNumber()->IntegerValue());
  }
  NodeAlbum* nodeAlbum = Nan::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodeAlbum->album->coverBase64(imageSize).c_str()).ToLocalChecked());
}

NAN_METHOD(NodeAlbum::browse) {
  NodeAlbum* nodeAlbum = Nan::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  if(nodeAlbum->album->albumBrowse == nullptr) {
    nodeAlbum->makePersistent();
    nodeAlbum->browseCompleteCallback.SetFunction(info[0].As<Function>());

    //Mutate the V8 object.
    Local<Object> nodeAlbumV8 = nodeAlbum->handle();
    Nan::SetAccessor(nodeAlbumV8, Nan::New<String>("tracks").ToLocalChecked(), getTracks);
    Nan::SetAccessor(nodeAlbumV8, Nan::New<String>("review").ToLocalChecked(), getReview);
    Nan::SetAccessor(nodeAlbumV8, Nan::New<String>("copyrights").ToLocalChecked(), getCopyrights);
    Nan::SetAccessor(nodeAlbumV8, Nan::New<String>("artist").ToLocalChecked(), getArtist);

    nodeAlbum->album->browse();
  } else {
    nodeAlbum->callBrowseComplete();
  }
  info.GetReturnValue().SetUndefined();
}

NAN_GETTER(NodeAlbum::getTracks) {
  NodeAlbum* nodeAlbum = Nan::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  std::vector<std::shared_ptr<Track>> tracks = nodeAlbum->album->tracks();
  Local<Array> nodeTracks = Nan::New<Array>(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    nodeTracks->Set(Nan::New<Number>(i), nodeTrack->createInstance());
  }
  info.GetReturnValue().Set(nodeTracks);
}

NAN_GETTER(NodeAlbum::getReview) {
  NodeAlbum* nodeAlbum = Nan::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodeAlbum->album->review().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeAlbum::getCopyrights) {
  NodeAlbum* nodeAlbum = Nan::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  std::vector<std::string> copyrights = nodeAlbum->album->copyrights();
  Local<Array> nodeCopyrights = Nan::New<Array>(copyrights.size());
  for(int i = 0; i < (int)copyrights.size(); i++) {
    nodeCopyrights->Set(Nan::New<Number>(i), Nan::New<String>(copyrights[i].c_str()).ToLocalChecked());
  }
  info.GetReturnValue().Set(nodeCopyrights);
}

NAN_GETTER(NodeAlbum::getArtist) {
  NodeAlbum* nodeAlbum = Nan::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  NodeArtist* nodeArtist = new NodeArtist(nodeAlbum->album->artist());
  info.GetReturnValue().Set(nodeArtist->createInstance());
}

NAN_GETTER(NodeAlbum::isLoaded) {
  NodeAlbum* nodeAlbum = Nan::ObjectWrap::Unwrap<NodeAlbum>(info.This());
  info.GetReturnValue().Set(Nan::New<Boolean>(nodeAlbum->album->isLoaded()));
}

void NodeAlbum::init() {
  Local<FunctionTemplate> constructorTemplate = NodeWrapped::init("Album");
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("name").ToLocalChecked(), getName);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("link").ToLocalChecked(), getLink);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("isLoaded").ToLocalChecked(), isLoaded);
  Nan::SetPrototypeMethod(constructorTemplate, "getCoverBase64", getCoverBase64);
  Nan::SetPrototypeMethod(constructorTemplate, "getCoverLink", getCoverLink);
  Nan::SetPrototypeMethod(constructorTemplate, "browse", browse);
  NodeAlbum::constructorTemplate.Reset(constructorTemplate);
}
