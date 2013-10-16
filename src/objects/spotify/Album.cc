#include "Album.h"
#include "../../utils/ImageUtils.h"
#include "../../Application.h"

extern Application* application;

std::map<sp_album*, std::shared_ptr<Album>> Album::cache;

Album::Album(sp_album* _album) : album(_album), coverBase64(0) {
  sp_album_add_ref(album);
  name = std::string(sp_album_name(album));
  
  const byte* coverId = sp_album_cover(album, SP_IMAGE_SIZE_NORMAL);
  if(coverId != 0) {
    sp_image* image = sp_image_create(application->session, coverId);
    if(sp_image_is_loaded(image)) {
      coverBase64 = ImageUtils::convertImageToBase64(image);
      sp_image_release(image);
    } else {
      sp_image_add_load_callback(image, ImageUtils::imageLoadedCallback, &this->coverBase64);
    }
  }
  char linkChar[256];
  sp_link* spLink = sp_link_create_from_album(album);
  sp_link_as_string(spLink, linkChar, 256);
  link = std::string(linkChar);
  sp_link_release(spLink);
};

void Album::clearCache() {
  Album::cache.clear();
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