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
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  NanReturnValue(NanNew<String>(nodeArtist->artist->name().c_str()));
}

NAN_GETTER(NodeArtist::getLink) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  NanReturnValue(NanNew<String>(nodeArtist->artist->link().c_str()));
}

NAN_METHOD(NodeArtist::browse) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  if(nodeArtist->artist->artistBrowse == nullptr) {
    nodeArtist->makePersistent();
    sp_artistbrowse_type artistbrowseType = static_cast<sp_artistbrowse_type>(args[0]->ToNumber()->IntegerValue());
    nodeArtist->browseCompleteCallback = std::unique_ptr<NanCallback>(new NanCallback(args[1].As<Function>()));

    //Mutate the V8 object.
    Handle<Object> nodeArtistV8 = NanObjectWrapHandle(nodeArtist);
    nodeArtistV8->SetAccessor(NanNew<String>("tracks"), getTracks);
    nodeArtistV8->SetAccessor(NanNew<String>("tophitTracks"), getTophitTracks);
    nodeArtistV8->SetAccessor(NanNew<String>("albums"), getAlbums);
    nodeArtistV8->SetAccessor(NanNew<String>("similarArtists"), getSimilarArtists);
    nodeArtistV8->SetAccessor(NanNew<String>("biography"), getBiography);
    //TODO: portraits

    nodeArtist->artist->browse(artistbrowseType);
  } else {
    nodeArtist->callBrowseComplete();
  }
  NanReturnUndefined();
}

NAN_GETTER(NodeArtist::getTracks) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  std::vector<std::shared_ptr<Track>> tracks = nodeArtist->artist->tracks();
  Local<Array> nodeTracks = NanNew<Array>(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    nodeTracks->Set(NanNew<Number>(i), nodeTrack->createInstance());
  }
  NanReturnValue(nodeTracks);
}

NAN_GETTER(NodeArtist::getTophitTracks) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  std::vector<std::shared_ptr<Track>> tophitTracks = nodeArtist->artist->tophitTracks();
  Local<Array> nodeTophitTracks = NanNew<Array>(tophitTracks.size());
  for(int i = 0; i < (int)tophitTracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tophitTracks[i]);
    nodeTophitTracks->Set(NanNew<Number>(i), nodeTrack->createInstance());
  }
  NanReturnValue(nodeTophitTracks);
}

NAN_GETTER(NodeArtist::getAlbums) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  std::vector<std::unique_ptr<Album>> albums = nodeArtist->artist->albums();
  Local<Array> nodeAlbums = NanNew<Array>(albums.size());
  for(int i = 0; i < (int)albums.size(); i++) {
    NodeAlbum* nodeAlbum = new NodeAlbum(std::move(albums[i]));
    nodeAlbums->Set(NanNew<Number>(i), nodeAlbum->createInstance());
  }
  NanReturnValue(nodeAlbums);
}

NAN_GETTER(NodeArtist::getSimilarArtists) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  std::vector<std::unique_ptr<Artist>> similarArtists = nodeArtist->artist->similarArtists();
  Local<Array> nodeSimilarArtists = NanNew<Array>(similarArtists.size());
  for(int i = 0; i < (int)similarArtists.size(); i++) {
    NodeArtist* nodeArtist = new NodeArtist(std::move(similarArtists[i]));
    nodeSimilarArtists->Set(NanNew<Number>(i), nodeArtist->createInstance());
  }
  NanReturnValue(nodeSimilarArtists);
}

NAN_GETTER(NodeArtist::getBiography) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  std::string biography = nodeArtist->artist->biography();
  NanReturnValue(NanNew<String>(biography.c_str()));
}

NAN_GETTER(NodeArtist::isLoaded) {
  NanScope();
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  NanReturnValue(NanNew<Boolean>(nodeArtist->artist->isLoaded()));
}

void NodeArtist::init() {
  NanScope();
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Artist");
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("name"), getName);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("link"), getLink);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("isLoaded"), isLoaded);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "browse", browse);
  NanAssignPersistent(NodeArtist::constructorTemplate, constructorTemplate);
}
