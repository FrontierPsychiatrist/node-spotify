#include "Album.h"
#include "base64.h"

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