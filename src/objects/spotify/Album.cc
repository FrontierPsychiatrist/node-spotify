#include "Album.h"
#include "base64.h"

std::map<sp_album*, std::shared_ptr<Album>> Album::cache;

void Album::processImage(sp_image* image) {
  size_t imageSize;
  int base64Size;
  const void* imageData = sp_image_data(image, &imageSize);
  this->coverBase64 = base64(imageData, (int)imageSize, &base64Size);
}

void imageLoadedCallback(sp_image* image, void* userdata) {
  Album* album = static_cast<Album*>(userdata);
  album->processImage(image);
  sp_image_remove_load_callback(image, &imageLoadedCallback, userdata);
  sp_image_release(image);
}

std::shared_ptr<Album> Album::fromCache(sp_album* album) {
  auto it = Album::cache.find(album);
  if( it != Album::cache.end()) {
    return it->second;
  } else {
    auto albumPointer = std::make_shared<Album>(album);
    Album::cache[album] = albumPointer;
    return albumPointer;
  }
}