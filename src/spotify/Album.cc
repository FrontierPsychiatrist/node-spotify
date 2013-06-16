#include "Album.h"
#include "base64.h"

std::map<sp_album*, Album*> Album::albumCache;

Handle<Value> Album::getName(Local<String> property, const AccessorInfo& info) {
  Album* album = node::ObjectWrap::Unwrap<Album>(info.Holder());
  return String::New(album->name.c_str());
}

Handle<Value> Album::getBase64Cover(const Arguments& args) {
  HandleScope scope;
  Album* album = node::ObjectWrap::Unwrap<Album>(args.This());
  if(album->screenshot == 0) {
    Album::simpleCall(args, &Album::spLoadScreenshot);
  } else {
    return scope.Close(String::New(album->screenshot));
  }
  return scope.Close(Undefined());
}

void Album::init(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
  constructorTemplate->SetClassName(String::NewSymbol("Album"));
  constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, emptySetter);

  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getBase64Cover", getBase64Cover);
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

void imageLoaded(sp_image* image, void* album) {
  size_t length;
  const void* rawImage = sp_image_data(image, &length);
  int outLength;
  ((Album*)album)->screenshot = base64(rawImage, (int)length, &outLength);
}

void Album::spLoadScreenshot() {
  const byte* coverId = sp_album_cover(spAlbum, SP_IMAGE_SIZE_NORMAL);
  sp_image* image = sp_image_create(spotifyService->spotifySession, coverId);
  sp_image_add_load_callback(image, &imageLoaded, (void*)this);
}