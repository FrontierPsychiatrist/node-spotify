#include "NodeSearch.h"
#include "NodeTrack.h"
#include "NodeAlbum.h"
#include "NodeArtist.h"
#include "NodePlaylist.h"
#include "../../Application.h"

extern Application* application;

NodeSearch::NodeSearch(const char* _searchQuery) : searchQuery(_searchQuery), trackOffset(0), albumOffset(0), artistOffset(0), playlistOffset(0),
  trackLimit(10), albumLimit(10), artistLimit(10), playlistLimit(10) {

}

NodeSearch::NodeSearch(const char* _searchQuery, int offset) : searchQuery(_searchQuery), trackOffset(offset), albumOffset(offset), artistOffset(offset), playlistOffset(offset),
  trackLimit(10), albumLimit(10), artistLimit(10), playlistLimit(10) {

}

NodeSearch::NodeSearch(const char* _searchQuery, int offset, int limit) : searchQuery(_searchQuery), trackOffset(offset), albumOffset(offset), artistOffset(offset), playlistOffset(offset),
  trackLimit(limit), albumLimit(limit), artistLimit(limit), playlistLimit(limit) {

}

NAN_METHOD(NodeSearch::execute) {
  NanScope();
  if(args.Length() < 1) {//TODO: how to check if it is a function? ->IsFunction() does not work, it does not recoginze functions.
    return NanThrowError("execute needs a callback function as its argument.");
  }
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->makePersistent();
  nodeSearch->browseCompleteCallback = std::unique_ptr<NanCallback>(new NanCallback(args[0].As<Function>()));
  nodeSearch->search = std::unique_ptr<Search>(new Search());
  nodeSearch->search->nodeObject = nodeSearch;
  nodeSearch->search->execute(nodeSearch->searchQuery, nodeSearch->trackOffset, nodeSearch->trackLimit,
    nodeSearch->albumOffset, nodeSearch->albumLimit,
    nodeSearch->artistOffset, nodeSearch->artistLimit,
    nodeSearch->playlistLimit, nodeSearch->playlistLimit);
  nodeSearch->setupAdditionalMethods();
  NanReturnUndefined();
}

/**
 * Adds adiitional properties to the V8 object.
 * These will call libspotify functions and should first be available when the search has been executed.
 **/
void NodeSearch::setupAdditionalMethods() {
  Handle<Object> nodeObject = NanObjectWrapHandle(this);
  nodeObject->SetAccessor(NanNew<String>("didYouMean"), didYouMean);
  nodeObject->SetAccessor(NanNew<String>("link"), getLink);
  nodeObject->Set(NanNew<String>("getTrack"), NanNew<FunctionTemplate>(getTrack)->GetFunction());
  nodeObject->Set(NanNew<String>("getAlbum"), NanNew<FunctionTemplate>(getAlbum)->GetFunction());
  nodeObject->Set(NanNew<String>("getArtist"), NanNew<FunctionTemplate>(getArtist)->GetFunction());
  nodeObject->Set(NanNew<String>("getPlaylist"), NanNew<FunctionTemplate>(getPlaylist)->GetFunction());
  nodeObject->SetAccessor(NanNew<String>("totalTracks"), getTotalTracks);
  nodeObject->SetAccessor(NanNew<String>("numTracks"), getNumTracks);
  nodeObject->SetAccessor(NanNew<String>("totalAlbums"), getTotalAlbums);
  nodeObject->SetAccessor(NanNew<String>("numAlbums"), getNumAlbums);
  nodeObject->SetAccessor(NanNew<String>("totalArtists"), getTotalArtists);
  nodeObject->SetAccessor(NanNew<String>("numArtists"), getNumArtists);
  nodeObject->SetAccessor(NanNew<String>("totalPlaylists"), getTotalPlaylists);
  nodeObject->SetAccessor(NanNew<String>("numPlaylists"), getNumPlaylists);
}

NAN_GETTER(NodeSearch::getTrackOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->trackOffset));
}

NAN_SETTER(NodeSearch::setTrackOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->trackOffset = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getAlbumOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->albumOffset));
}

NAN_SETTER(NodeSearch::setAlbumOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->albumOffset = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getArtistOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->artistOffset));
}

NAN_SETTER(NodeSearch::setArtistOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->artistOffset = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getPlaylistOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->playlistOffset));
}

NAN_SETTER(NodeSearch::setPlaylistOffset) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->playlistOffset = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getTrackLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->trackLimit));
}

NAN_SETTER(NodeSearch::setTrackLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->trackLimit = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getAlbumLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->albumLimit));
}

NAN_SETTER(NodeSearch::setAlbumLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->albumLimit = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getArtistLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->artistLimit));
}

NAN_SETTER(NodeSearch::setArtistLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->artistLimit = value->ToInteger()->Value();
}

NAN_GETTER(NodeSearch::getPlaylistLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->playlistLimit));
}

NAN_SETTER(NodeSearch::setPlaylistLimit) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  nodeSearch->playlistLimit = value->ToInteger()->Value();
}

NAN_METHOD(NodeSearch::New) {
  NanScope();
  NodeSearch* search;
  NanUtf8String searchQuery(args[0]->ToString());
  if(args.Length() == 1) {
    search = new NodeSearch(*searchQuery);
  } else if(args.Length() == 2) {
    int offset = args[1]->ToInteger()->Value();
    search = new NodeSearch(*searchQuery, offset);
  } else if(args.Length() == 3) {
    int offset = args[1]->ToInteger()->Value();
    int limit = args[2]->ToInteger()->Value();
    search = new NodeSearch(*searchQuery, offset, limit);
  } else {
    return NanThrowError("Please provide an object to the node-spotify initializer function");
  }
  search->Wrap(args.This());
  NanReturnThis();
}

NAN_GETTER(NodeSearch::didYouMean) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<String>(nodeSearch->search->didYouMeanText().c_str()));
}

NAN_GETTER(NodeSearch::getLink) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<String>(nodeSearch->search->link().c_str()));
}

NAN_METHOD(NodeSearch::getTrack) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return NanThrowError("getTrack needs a number as its first argument.");
  }

  int position = args[0]->ToNumber()->IntegerValue();
  if(position >= nodeSearch->search->numTracks() || position < 0) {
    return NanThrowError("Track index out of bounds");
  }

  NodeTrack* nodeTrack = new NodeTrack(nodeSearch->search->getTrack(position));
  NanReturnValue(nodeTrack->createInstance());
}

NAN_METHOD(NodeSearch::getAlbum) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return NanThrowError("getAlbum needs a number as its first argument.");
  }

  int position = args[0]->ToNumber()->IntegerValue();
  if(position >= nodeSearch->search->numAlbums() || position < 0) {
    return NanThrowError("Album index out of bounds");
  }

  NodeAlbum* nodeAlbum = new NodeAlbum(nodeSearch->search->getAlbum(position));
  NanReturnValue(nodeAlbum->createInstance());
}

NAN_METHOD(NodeSearch::getArtist) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return NanThrowError("getArtist needs a number as its first argument.");
  }

  int position = args[0]->ToNumber()->IntegerValue();
  if(position >= nodeSearch->search->numArtists() || position < 0) {
    return NanThrowError("Artist index out of bounds");
  }

  NodeArtist* nodeArtist = new NodeArtist(nodeSearch->search->getArtist(position));
  NanReturnValue(nodeArtist->createInstance());
}

NAN_METHOD(NodeSearch::getPlaylist) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  if(args.Length() < 1 || !args[0]->IsNumber()) {
    return NanThrowError("getPlaylist needs a number as its first argument.");
  }

  int position = args[0]->ToNumber()->IntegerValue();
  if(position >= nodeSearch->search->numPlaylists() || position < 0) {
    return NanThrowError("Playlist index out of bounds");
  }

  NodePlaylist* nodePlaylist = new NodePlaylist(nodeSearch->search->getPlaylist(position));
  NanReturnValue(nodePlaylist->createInstance());
}

NAN_GETTER(NodeSearch::getTotalTracks) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->search->totalTracks()));
}

NAN_GETTER(NodeSearch::getNumTracks) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->search->numTracks()));
}

NAN_GETTER(NodeSearch::getTotalAlbums) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->search->totalAlbums()));
}

NAN_GETTER(NodeSearch::getNumAlbums) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->search->numAlbums()));
}

NAN_GETTER(NodeSearch::getTotalArtists) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->search->totalArtists()));
}

NAN_GETTER(NodeSearch::getNumArtists) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->search->numArtists()));
}

NAN_GETTER(NodeSearch::getTotalPlaylists) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->search->totalPlaylists()));
}

NAN_GETTER(NodeSearch::getNumPlaylists) {
  NanScope();
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  NanReturnValue(NanNew<Integer>(nodeSearch->search->numPlaylists()));
}

void NodeSearch::init() {
  NanScope();
  Local<FunctionTemplate> constructorTemplate = NanNew<FunctionTemplate>(New);
  constructorTemplate->SetClassName(NanNew<String>("Search"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "execute", execute);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("trackOffset"), getTrackOffset, setTrackOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("trackLimit"), getTrackLimit, setTrackLimit);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("albumOffset"), getAlbumOffset, setAlbumOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("albumLimit"), getAlbumLimit, setAlbumLimit);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("artistOffset"), getArtistOffset, setArtistOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("artistLimit"), getArtistLimit, setArtistLimit);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("playlistOffset"), getPlaylistOffset, setPlaylistOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("playlistLimit"), getPlaylistLimit, setPlaylistLimit);
  NanAssignPersistent(NodeSearch::constructorTemplate, constructorTemplate);
}
