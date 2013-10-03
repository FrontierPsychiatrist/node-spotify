#ifndef _ALBUM_H
#define _ALBUM_H

#include <libspotify/api.h>
#include <string>

#include "../../Application.h"

extern Application* application;

void imageLoadedCallback(sp_image* image, void* userdata);

class Album {
friend class NodeAlbum;
public:
  Album(sp_album* _album) : album(_album), coverBase64(0) {
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
  };
  ~Album() {
    sp_album_release(album);
  };
  Album(const Album& other) : name(other.name), album(other.album), coverBase64(other.coverBase64) {
    sp_album_add_ref(album);
  };
  void processImage(sp_image* image);
private:
  std::string name;
  sp_album* album;
  char* coverBase64;
};

#endif