#include "Artist.h"
std::map<sp_artist*, Artist*> Artist::artistCache;

Handle<Value> Artist::getName(Local<String> property, const AccessorInfo& info) {
  Artist* artist = node::ObjectWrap::Unwrap<Artist>(info.Holder());
  return String::New(artist->name.c_str());
}

void Artist::init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("Artist"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, emptySetter);

  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
}

Artist* Artist::getArtist(sp_artist* _spArtist) { 
  auto it = Artist::artistCache.find(_spArtist);
  if(it != Artist::artistCache.end()) {
    return it->second;
  } else {
    return 0;
  }
}

void Artist::putArtist(Artist* artist) {
  Artist::artistCache[artist->spArtist] = artist;
}
