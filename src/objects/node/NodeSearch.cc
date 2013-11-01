#include "NodeSearch.h"
#include "NodeTrack.h"
#include "NodeAlbum.h"
#include "NodeArtist.h"
#include "NodePlaylist.h"
#include "../../callbacks/SearchCallbacks.h"
#include "../../events.h"
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

void NodeSearch::setSearch(std::shared_ptr<Search> _search) {
  this->search = _search;
}

Handle<Value> NodeSearch::execute(const Arguments& args) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  Persistent<Function> callback = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
  nodeSearch->on(SEARCH_COMPLETE, callback);
  sp_search_create(application->session,
                  nodeSearch->searchQuery.c_str(),
                  nodeSearch->trackOffset, nodeSearch->trackLimit,
                  nodeSearch->albumOffset, nodeSearch->albumLimit,
                  nodeSearch->artistOffset, nodeSearch->artistLimit,
                  nodeSearch->playlistOffset, nodeSearch->playlistLimit,
                  SP_SEARCH_STANDARD, //?
                  SearchCallbacks::searchComplete,
                  nodeSearch
                  );
  return scope.Close(Undefined());
}

Handle<Value> NodeSearch::getTrackOffset(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->trackOffset));
}

void NodeSearch::setTrackOffset(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  nodeSearch->trackOffset = value->ToInteger()->Value();
  scope.Close(Undefined());
}

Handle<Value> NodeSearch::getAlbumOffset(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->albumOffset));
}

void NodeSearch::setAlbumOffset(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  nodeSearch->albumOffset = value->ToInteger()->Value();
  scope.Close(Undefined());
}

Handle<Value> NodeSearch::getArtistOffset(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->artistOffset));
}

void NodeSearch::setArtistOffset(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  nodeSearch->artistOffset = value->ToInteger()->Value();
  scope.Close(Undefined());
}

Handle<Value> NodeSearch::getPlaylistOffset(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->playlistOffset));
}

void NodeSearch::setPlaylistOffset(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  nodeSearch->playlistOffset = value->ToInteger()->Value();
  scope.Close(Undefined());
}

Handle<Value> NodeSearch::getTrackLimit(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->trackLimit));
}

void NodeSearch::setTrackLimit(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  nodeSearch->trackLimit = value->ToInteger()->Value();
  scope.Close(Undefined());
}

Handle<Value> NodeSearch::getAlbumLimit(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->albumLimit));
}

void NodeSearch::setAlbumLimit(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  nodeSearch->albumLimit = value->ToInteger()->Value();
  scope.Close(Undefined());
}

Handle<Value> NodeSearch::getArtistLimit(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->artistLimit));
}

void NodeSearch::setArtistLimit(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  nodeSearch->artistLimit = value->ToInteger()->Value();
  scope.Close(Undefined());
}

Handle<Value> NodeSearch::getPlaylistLimit(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->playlistLimit));
}

void NodeSearch::setPlaylistLimit(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  nodeSearch->playlistLimit = value->ToInteger()->Value();
  scope.Close(Undefined());
}

Handle<Value> NodeSearch::New(const Arguments& args) {
  HandleScope scope;
  NodeSearch* search;
  String::Utf8Value searchQuery(args[0]->ToString());
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
    return scope.Close(ThrowException(Exception::Error(String::New("Wrong arguments to search"))));
  }
  search->Wrap(args.This());
  return scope.Close(args.This());
}

Handle<Value> NodeSearch::didYouMean(Local<String> property, const AccessorInfo& info) {
    HandleScope scope;
    NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
    return scope.Close(String::New(nodeSearch->search->didYouMeanText().c_str()));
}

Handle<Value> NodeSearch::getLink(Local<String> property, const AccessorInfo& info) {
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return String::New(nodeSearch->search->link().c_str());
}

Handle<Value> NodeSearch::getTracks(const Arguments& args) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  std::vector<std::shared_ptr<Track>> tracks = nodeSearch->search->getTracks();
  Local<Array> outArray = Array::New(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    outArray->Set(Number::New(i), nodeTrack->getV8Object());
  }
  return scope.Close(outArray);
}

Handle<Value> NodeSearch::getAlbums(const Arguments& args) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  std::vector<std::shared_ptr<Album>> albums = nodeSearch->search->getAlbums();
  Local<Array> outArray = Array::New(albums.size());
  for(int i = 0; i < (int)albums.size(); i++) {
    NodeAlbum* nodeAlbum = new NodeAlbum(albums[i]);
    outArray->Set(Number::New(i), nodeAlbum->getV8Object());
  }
  return scope.Close(outArray);
}

Handle<Value> NodeSearch::getArtists(const Arguments& args) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  std::vector<std::shared_ptr<Artist>> artists = nodeSearch->search->getArtists();
  Local<Array> outArray = Array::New(artists.size());
  for(int i = 0; i < (int)artists.size(); i++) {
    NodeArtist* nodeArtist = new NodeArtist(artists[i]);
    outArray->Set(Number::New(i), nodeArtist->getV8Object());
  }
  return scope.Close(outArray);
}

Handle<Value> NodeSearch::getPlaylists(const Arguments& args) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(args.This());
  std::vector<std::shared_ptr<Playlist>> playlists = nodeSearch->search->getPlaylists();
  Local<Array> outArray = Array::New(playlists.size());
  for(int i = 0; i < (int)playlists.size(); i++) {
    NodePlaylist* nodePlaylist = new NodePlaylist(playlists[i]);
    outArray->Set(Number::New(i), nodePlaylist->getV8Object());
  }
  return scope.Close(outArray);
}

Handle<Value> NodeSearch::getTotalTracks(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->search->totalTracks));
}

Handle<Value> NodeSearch::getTotalAlbums(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->search->totalAlbums));
}

Handle<Value> NodeSearch::getTotalArtists(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->search->totalArtists));
}

Handle<Value> NodeSearch::getTotalPlaylists(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeSearch* nodeSearch = node::ObjectWrap::Unwrap<NodeSearch>(info.Holder());
  return scope.Close(Integer::New(nodeSearch->search->totalPlaylists));
}

Handle<Function> NodeSearch::getConstructor() {
  HandleScope scope;
  return scope.Close(constructor);
}

void NodeSearch::init() {
HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New(New);
  constructorTemplate->SetClassName(String::NewSymbol("Search"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "execute", execute);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("trackOffset"), getTrackOffset, setTrackOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("trackLimit"), getTrackLimit, setTrackLimit);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("albumOffset"), getAlbumOffset, setAlbumOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("albumLimit"), getAlbumLimit, setAlbumLimit);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("artistOffset"), getArtistOffset, setArtistOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("artistLimit"), getArtistLimit, setArtistLimit);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("playlistOffset"), getPlaylistOffset, setPlaylistOffset);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("playlistLimit"), getPlaylistLimit, setPlaylistLimit);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("didYouMean"), didYouMean, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("link"), getLink, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("totalTracks"), getTotalTracks, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("totalAlbums"), getTotalAlbums, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("totalArtists"), getTotalArtists, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("totalPlaylists"), getTotalPlaylists, emptySetter);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getTracks", getTracks);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getAlbums", getAlbums);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getArtists", getArtists);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getPlaylists", getPlaylists);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}