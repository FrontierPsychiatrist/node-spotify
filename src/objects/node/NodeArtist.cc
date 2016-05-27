#include "NodeArtist.h"
#include "NodeTrack.h"
#include "NodeAlbum.h"

NodeArtist::NodeArtist(std::unique_ptr<Artist> _artist) : artist(std::move(_artist)) {
  artist->nodeObject = this;
}

NodeArtist::~NodeArtist() {
  if(artist->nodeObject == this) {
    artist->nodeObject = nullptr;
  }
}

NAN_GETTER(NodeArtist::getName) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  NanReturnValue(Nan::New<String>(nodeArtist->artist->name().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeArtist::getLink) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  NanReturnValue(Nan::New<String>(nodeArtist->artist->link().c_str()).ToLocalChecked());
}

NAN_METHOD(NodeArtist::browse) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  if(nodeArtist->artist->artistBrowse == nullptr) {
    nodeArtist->makePersistent();
    sp_artistbrowse_type artistbrowseType = static_cast<sp_artistbrowse_type>(args[0]->ToNumber()->IntegerValue());
    nodeArtist->browseCompleteCallback.SetFunction(args[1].As<Function>());

    //Mutate the V8 object.
    Handle<Object> nodeArtistV8 = NanObjectWrapHandle(nodeArtist);
    nodeArtistV8->SetAccessor(Nan::New<String>("tracks").ToLocalChecked(), getTracks);
    nodeArtistV8->SetAccessor(Nan::New<String>("tophitTracks").ToLocalChecked(), getTophitTracks);
    nodeArtistV8->SetAccessor(Nan::New<String>("albums").ToLocalChecked(), getAlbums);
    nodeArtistV8->SetAccessor(Nan::New<String>("similarArtists").ToLocalChecked(), getSimilarArtists);
    nodeArtistV8->SetAccessor(Nan::New<String>("biography").ToLocalChecked(), getBiography);
    //TODO: portraits

    nodeArtist->artist->browse(artistbrowseType);
  } else {
    nodeArtist->callBrowseComplete();
  }
  NanReturnUndefined();
}

NAN_GETTER(NodeArtist::getTracks) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  std::vector<std::shared_ptr<Track>> tracks = nodeArtist->artist->tracks();
  Local<Array> nodeTracks = NanNew<Array>(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    nodeTracks->Set(NanNew<Number>(i), nodeTrack->createInstance());
  }
  NanReturnValue(nodeTracks);
}

NAN_GETTER(NodeArtist::getTophitTracks) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  std::vector<std::shared_ptr<Track>> tophitTracks = nodeArtist->artist->tophitTracks();
  Local<Array> nodeTophitTracks = NanNew<Array>(tophitTracks.size());
  for(int i = 0; i < (int)tophitTracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tophitTracks[i]);
    nodeTophitTracks->Set(NanNew<Number>(i), nodeTrack->createInstance());
  }
  NanReturnValue(nodeTophitTracks);
}

NAN_GETTER(NodeArtist::getAlbums) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  std::vector<std::unique_ptr<Album>> albums = nodeArtist->artist->albums();
  Local<Array> nodeAlbums = NanNew<Array>(albums.size());
  for(int i = 0; i < (int)albums.size(); i++) {
    NodeAlbum* nodeAlbum = new NodeAlbum(std::move(albums[i]));
    nodeAlbums->Set(NanNew<Number>(i), nodeAlbum->createInstance());
  }
  NanReturnValue(nodeAlbums);
}

NAN_GETTER(NodeArtist::getSimilarArtists) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  std::vector<std::unique_ptr<Artist>> similarArtists = nodeArtist->artist->similarArtists();
  Local<Array> nodeSimilarArtists = NanNew<Array>(similarArtists.size());
  for(int i = 0; i < (int)similarArtists.size(); i++) {
    NodeArtist* nodeArtist = new NodeArtist(std::move(similarArtists[i]));
    nodeSimilarArtists->Set(NanNew<Number>(i), nodeArtist->createInstance());
  }
  NanReturnValue(nodeSimilarArtists);
}

NAN_GETTER(NodeArtist::getBiography) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  std::string biography = nodeArtist->artist->biography();
  NanReturnValue(Nan::New<String>(biography.c_str()).ToLocalChecked());
}

NAN_GETTER(NodeArtist::isLoaded) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  NanReturnValue(NanNew<Boolean>(nodeArtist->artist->isLoaded()));
}

void NodeArtist::init() {
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Artist");
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("name").ToLocalChecked(), getName);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("link").ToLocalChecked(), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(Nan::New<String>("isLoaded").ToLocalChecked(), isLoaded);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "browse", browse);
  NanAssignPersistent(NodeArtist::constructorTemplate, constructorTemplate);
}
