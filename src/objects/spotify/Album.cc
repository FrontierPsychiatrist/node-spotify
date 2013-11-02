#include "Album.h"
#include "../../utils/ImageUtils.h"
#include "../../Application.h"
#include "../../callbacks/AlbumBrowseCallbacks.h"

extern Application* application;

Album::Album(sp_album* _album) : album(_album), cover(nullptr), nodeObject(nullptr), albumBrowse(nullptr) {
  sp_album_add_ref(album);
};

std::string Album::name() {
  std::string name;
  if(sp_album_is_loaded(album)) {
    name = std::string(sp_album_name(album));
  } else {
    name = "Loading...";
  }
  return name;
}

std::string Album::link() {
  std::string link;
  if(sp_album_is_loaded(album)) {
    char linkChar[256];
    sp_link* spLink = sp_link_create_from_album(album);
    sp_link_as_string(spLink, linkChar, 256);
    link = std::string(linkChar);
    sp_link_release(spLink);
  }
  return link;
}

std::string Album::coverBase64() {
  std::string cover;
  if(sp_album_is_loaded(album)) {
    const byte* coverId = sp_album_cover(album, SP_IMAGE_SIZE_NORMAL);
    if(coverId != nullptr) {
      sp_image* image = sp_image_create(application->session, coverId);
      if(sp_image_is_loaded(image)) {
        this->cover = image;
        cover = std::string(ImageUtils::convertImageToBase64(image));
      } else {
        cover = "Cover loading...";
        sp_image_add_load_callback(image, ImageUtils::imageLoadedCallback, nullptr);
      }
    }
  } else {
    cover = "Album loading...";
  }
  return cover;
}

void Album::browse() {
  sp_albumbrowse_create(application->session, album, AlbumBrowseCallbacks::albumBrowseComplete, this);
}