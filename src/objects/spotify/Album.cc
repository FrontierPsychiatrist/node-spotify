#include "Album.h"
#include "base64.h"
#include "../../Application.h"

extern Application* application;

static void imageLoadedCallback(sp_image* image, void* userdata);

std::map<sp_album*, std::shared_ptr<Album>> Album::cache;

Album::Album(sp_album* _album) : album(_album), coverBase64(0) {
  sp_album_add_ref(album);
  name = std::string(sp_album_name(album));
  
  const byte* coverId = sp_album_cover(album, SP_IMAGE_SIZE_NORMAL);
  if(coverId != 0) {
    sp_image* image = sp_image_create(application->session, coverId);
    if(sp_image_is_loaded(image)) {
      processImage(image);
      sp_image_release(image);
    } else {
      sp_image_add_load_callback(image, &imageLoadedCallback, this);
    }
  }
  char linkChar[256];
  sp_link* spLink = sp_link_create_from_album(album);
  sp_link_as_string(spLink, linkChar, 256);
  link = linkChar;
  sp_link_release(spLink);
};

void Album::processImage(sp_image* image) {
  size_t imageSize;
  int base64Size;
  const void* imageData = sp_image_data(image, &imageSize);
  this->coverBase64 = base64(imageData, (int)imageSize, &base64Size);
}

static void imageLoadedCallback(sp_image* image, void* userdata) {
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