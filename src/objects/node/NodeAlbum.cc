#include "NodeAlbum.h"
#include "NodeTrack.h"
#include "NodeArtist.h"
#include "../spotify/Artist.h"
#include "../spotify/Track.h"
#include "../../events.h"

Handle<Value> NodeAlbum::getName(Local<String> property, const AccessorInfo& info) {
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.Holder());
  return String::New(nodeAlbum->album->name().c_str());
}

Handle<Value> NodeAlbum::getLink(Local<String> property, const AccessorInfo& info) {
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.Holder());
  return String::New(nodeAlbum->album->link().c_str());
}

Handle<Value> NodeAlbum::getCoverBase64(const Arguments& args) {
  HandleScope scope;
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(args.This());
  return scope.Close(String::New(nodeAlbum->album->coverBase64().c_str()));
}

Handle<Value> NodeAlbum::browse(const Arguments& args) {
  HandleScope scope;
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(args.This());
  if(nodeAlbum->album->albumBrowse == nullptr) {
    Persistent<Function> callback = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
    nodeAlbum->on(ALBUMBROWSE_COMPLETE, callback);

    //Mutate the V8 object. TODO: is this permanent, i.e. available after this scope closed?
    Handle<Object> nodeAlbumV8 = nodeAlbum->getV8Object();
    nodeAlbumV8->SetAccessor(String::NewSymbol("tracks"), getTracks);
    nodeAlbumV8->SetAccessor(String::NewSymbol("review"), getReview);
    nodeAlbumV8->SetAccessor(String::NewSymbol("copyrights"), getCopyrights);
    nodeAlbumV8->SetAccessor(String::NewSymbol("artist"), getArtist);

    nodeAlbum->album->browse();
  } else {
    nodeAlbum->call(ALBUMBROWSE_COMPLETE);
  }
  return scope.Close(Undefined());
}
//TODO: Delegate to Album, no libspotify api calls here!
Handle<Value> NodeAlbum::getTracks(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.Holder());
  int numTracks = sp_albumbrowse_num_tracks(nodeAlbum->album->albumBrowse);
  Handle<Array> nodeTracks = Array::New(numTracks);
  for(int i = 0; i < numTracks; i++) {
    NodeTrack* nodeTrack = new NodeTrack(std::make_shared<Track>(sp_albumbrowse_track(nodeAlbum->album->albumBrowse, i)));
    nodeTracks->Set(Number::New(i), nodeTrack->getV8Object());
  }
  return scope.Close(nodeTracks);
}

Handle<Value> NodeAlbum::getReview(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.Holder());
  Handle<String> review = String::New(sp_albumbrowse_review(nodeAlbum->album->albumBrowse));
  return scope.Close(review);
}

Handle<Value> NodeAlbum::getCopyrights(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.Holder());
  int numCopyrights = sp_albumbrowse_num_copyrights(nodeAlbum->album->albumBrowse);
  Handle<Array> copyrights = Array::New(numCopyrights);
  for(int i = 0; i < numCopyrights; i++) {
    copyrights->Set(Number::New(i), String::New(sp_albumbrowse_copyright(nodeAlbum->album->albumBrowse, i)));
  }
  return scope.Close(copyrights);
}

Handle<Value> NodeAlbum::getArtist(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeAlbum* nodeAlbum = node::ObjectWrap::Unwrap<NodeAlbum>(info.Holder());
  NodeArtist* nodeArtist = new NodeArtist(std::make_shared<Artist>(sp_albumbrowse_artist(nodeAlbum->album->albumBrowse)));
  return scope.Close(nodeArtist->getV8Object());
}

void NodeAlbum::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Album");
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("link"), getLink, emptySetter);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getCoverBase64", getCoverBase64);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "browse", browse);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}