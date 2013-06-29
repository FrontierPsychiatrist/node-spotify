#include "Album.h"
#include "base64.h"

std::map<sp_album*, Album*> Album::albumCache;

Handle<Value> Album::getName(Local<String> property, const AccessorInfo& info) {
  Album* album = node::ObjectWrap::Unwrap<Album>(info.Holder());
  return String::New(album->name.c_str());
}

void Album::init(Handle<Object> target) {
  HandleScope scope;
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Album");
  constructorTemplate->InstanceTemplate()->SetAccessor(String::NewSymbol("name"), getName, emptySetter);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "getCoverBase64", getCoverBase64);
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

Handle<Value> Album::getCoverBase64(const Arguments& args) {
  Album* album = node::ObjectWrap::Unwrap<Album>(args.This());
  if(album->coverBase64 != 0) {
    return String::New(album->coverBase64);
  }
  return Undefined();
}

void Album::processImage(sp_image* image) {
  pthread_mutex_lock(&lockingMutex);
  size_t imageSize;
  int base64Size;
  const void* imageData = sp_image_data(image, &imageSize);
  this->coverBase64 = base64(imageData, (int)imageSize, &base64Size);
  pthread_mutex_unlock(&lockingMutex);
}

void imageLoadedCallback(sp_image* image, void* userdata) {
  Album* album = static_cast<Album*>(userdata);
  album->processImage(image);
  //player->call(NOW_PLAYING_DATA_CHANGED);
  sp_image_remove_load_callback(image, &imageLoadedCallback, userdata);
  sp_image_release(image);
}