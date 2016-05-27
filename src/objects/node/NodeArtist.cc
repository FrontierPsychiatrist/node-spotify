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
  info.GetReturnValue().Set(Nan::New<String>(nodeArtist->artist->name().c_str()).ToLocalChecked());
}

NAN_GETTER(NodeArtist::getLink) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  info.GetReturnValue().Set(Nan::New<String>(nodeArtist->artist->link().c_str()).ToLocalChecked());
}

NAN_METHOD(NodeArtist::browse) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  if(nodeArtist->artist->artistBrowse == nullptr) {
    nodeArtist->makePersistent();
    sp_artistbrowse_type artistbrowseType = static_cast<sp_artistbrowse_type>(info[0]->ToNumber()->IntegerValue());
    nodeArtist->browseCompleteCallback.SetFunction(info[1].As<Function>());

    //Mutate the V8 object.
    Handle<Object> nodeArtistV8 = nodeArtist->handle();
    Nan::SetAccessor(nodeArtistV8, Nan::New<String>("tracks").ToLocalChecked(), getTracks);
    Nan::SetAccessor(nodeArtistV8, Nan::New<String>("tophitTracks").ToLocalChecked(), getTophitTracks);
    Nan::SetAccessor(nodeArtistV8, Nan::New<String>("albums").ToLocalChecked(), getAlbums);
    Nan::SetAccessor(nodeArtistV8, Nan::New<String>("similarArtists").ToLocalChecked(), getSimilarArtists);
    Nan::SetAccessor(nodeArtistV8, Nan::New<String>("biography").ToLocalChecked(), getBiography);
    //TODO: portraits

    nodeArtist->artist->browse(artistbrowseType);
  } else {
    nodeArtist->callBrowseComplete();
  }
  info.GetReturnValue().SetUndefined();
}

NAN_GETTER(NodeArtist::getTracks) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  std::vector<std::shared_ptr<Track>> tracks = nodeArtist->artist->tracks();
  Local<Array> nodeTracks = Nan::New<Array>(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    nodeTracks->Set(Nan::New<Number>(i), nodeTrack->createInstance());
  }
  info.GetReturnValue().Set(nodeTracks);
}

NAN_GETTER(NodeArtist::getTophitTracks) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  std::vector<std::shared_ptr<Track>> tophitTracks = nodeArtist->artist->tophitTracks();
  Local<Array> nodeTophitTracks = Nan::New<Array>(tophitTracks.size());
  for(int i = 0; i < (int)tophitTracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tophitTracks[i]);
    nodeTophitTracks->Set(Nan::New<Number>(i), nodeTrack->createInstance());
  }
  info.GetReturnValue().Set(nodeTophitTracks);
}

NAN_GETTER(NodeArtist::getAlbums) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  std::vector<std::unique_ptr<Album>> albums = nodeArtist->artist->albums();
  Local<Array> nodeAlbums = Nan::New<Array>(albums.size());
  for(int i = 0; i < (int)albums.size(); i++) {
    NodeAlbum* nodeAlbum = new NodeAlbum(std::move(albums[i]));
    nodeAlbums->Set(Nan::New<Number>(i), nodeAlbum->createInstance());
  }
  info.GetReturnValue().Set(nodeAlbums);
}

NAN_GETTER(NodeArtist::getSimilarArtists) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  std::vector<std::unique_ptr<Artist>> similarArtists = nodeArtist->artist->similarArtists();
  Local<Array> nodeSimilarArtists = Nan::New<Array>(similarArtists.size());
  for(int i = 0; i < (int)similarArtists.size(); i++) {
    NodeArtist* nodeArtist = new NodeArtist(std::move(similarArtists[i]));
    nodeSimilarArtists->Set(Nan::New<Number>(i), nodeArtist->createInstance());
  }
  info.GetReturnValue().Set(nodeSimilarArtists);
}

NAN_GETTER(NodeArtist::getBiography) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  std::string biography = nodeArtist->artist->biography();
  info.GetReturnValue().Set(Nan::New<String>(biography.c_str()).ToLocalChecked());
}

NAN_GETTER(NodeArtist::isLoaded) {
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.This());
  info.GetReturnValue().Set(Nan::New<Boolean>(nodeArtist->artist->isLoaded()));
}

void NodeArtist::init() {
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Artist");
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("name").ToLocalChecked(), getName);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("link").ToLocalChecked(), getLink);
  Nan::SetAccessor(constructorTemplate->InstanceTemplate(), Nan::New<String>("isLoaded").ToLocalChecked(), isLoaded);
  Nan::SetMethod(constructorTemplate, "browse", browse);
  NodeArtist::constructorTemplate.Reset(constructorTemplate);
}
