/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#include "NodeArtist.h"
#include "NodeTrack.h"
#include "NodeAlbum.h"

#include "../../events.h"

Handle<Value> NodeArtist::getName(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());
  return scope.Close(String::New(nodeArtist->artist->name().c_str()));
}

Handle<Value> NodeArtist::getLink(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());
  return scope.Close(String::New(nodeArtist->artist->link().c_str()));
}

Handle<Value> NodeArtist::browse(const Arguments& args) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(args.This());
  if(nodeArtist->artist->artistBrowse == nullptr) {
    sp_artistbrowse_type artistbrowseType = static_cast<sp_artistbrowse_type>(args[0]->ToNumber()->IntegerValue());
    Persistent<Function> callback = Persistent<Function>::New(Handle<Function>::Cast(args[1]));
    nodeArtist->on(ALBUMBROWSE_COMPLETE, callback);

    //Mutate the V8 object.
    Handle<Object> nodeArtistV8 = nodeArtist->getV8Object();
    nodeArtistV8->SetAccessor(String::NewSymbol("tracks"), getTracks);
    nodeArtistV8->SetAccessor(String::NewSymbol("tophitTracks"), getTophitTracks);
    nodeArtistV8->SetAccessor(String::NewSymbol("albums"), getAlbums);
    nodeArtistV8->SetAccessor(String::NewSymbol("similarArtists"), getSimilarArtists);
    nodeArtistV8->SetAccessor(String::NewSymbol("biography"), getBiography);
    //TODO: portraits

    nodeArtist->artist->browse(artistbrowseType);
  } else {
    nodeArtist->call(ARTISTBROWSE_COMPLETE);
  }
  return scope.Close(Undefined());
}

Handle<Value> NodeArtist::getTracks(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());
  std::vector<std::shared_ptr<Track>> tracks = nodeArtist->artist->tracks();
  Local<Array> nodeTracks = Array::New(tracks.size());
  for(int i = 0; i < (int)tracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tracks[i]);
    nodeTracks->Set(Number::New(i), nodeTrack->getV8Object());
  }
  return scope.Close(nodeTracks);
}

Handle<Value> NodeArtist::getTophitTracks(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());
  std::vector<std::shared_ptr<Track>> tophitTracks = nodeArtist->artist->tophitTracks();
  Local<Array> nodeTophitTracks = Array::New(tophitTracks.size());
  for(int i = 0; i < (int)tophitTracks.size(); i++) {
    NodeTrack* nodeTrack = new NodeTrack(tophitTracks[i]);
    nodeTophitTracks->Set(Number::New(i), nodeTrack->getV8Object());
  }
  return scope.Close(nodeTophitTracks);
}

Handle<Value> NodeArtist::getAlbums(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());
  std::vector<std::shared_ptr<Album>> albums = nodeArtist->artist->albums();
  Local<Array> nodeAlbums = Array::New(albums.size());
  for(int i = 0; i < (int)albums.size(); i++) {
    NodeAlbum* nodeAlbum = new NodeAlbum(albums[i]);
    nodeAlbums->Set(Number::New(i), nodeAlbum->getV8Object());
  }
  return scope.Close(nodeAlbums);
}

Handle<Value> NodeArtist::getSimilarArtists(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());
  std::vector<std::shared_ptr<Artist>> similarArtists = nodeArtist->artist->similarArtists();
  Local<Array> nodeSimilarArtists = Array::New(similarArtists.size());
  for(int i = 0; i < (int)similarArtists.size(); i++) {
    NodeArtist* nodeArtist = new NodeArtist(similarArtists[i]);
    nodeSimilarArtists->Set(Number::New(i), nodeArtist->getV8Object());
  }
  return scope.Close(nodeSimilarArtists);
}

Handle<Value> NodeArtist::getBiography(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  NodeArtist* nodeArtist = node::ObjectWrap::Unwrap<NodeArtist>(info.Holder());
  std::string biography = nodeArtist->artist->biography();
  return scope.Close(String::New(biography.c_str()));
}

void NodeArtist::init() {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Artist");
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, emptySetter);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("link"), getLink, emptySetter);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "browse", browse);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}