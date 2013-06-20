#include "Album.h"

std::map<sp_album*, Album*> Album::albumCache;

Handle<Value> Album::getName(Local<String> property, const AccessorInfo& info) {
  Album* album = node::ObjectWrap::Unwrap<Album>(info.Holder());
  return String::New(album->name.c_str());
}

void Album::init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("Album"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, emptySetter);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}

Album* Album::getAlbum(sp_album* _spAlbum) {
  std::map<sp_album*, Album*>::iterator it = Album::albumCache.find(_spAlbum);
  if(it != Album::albumCache.end()) {
    return it->second;
  } else {
    return 0;
  }
}

void Album::putAlbum(Album* album) {
  Album::albumCache[album->spAlbum] = album;
}