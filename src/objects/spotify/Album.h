#ifndef _ALBUM_H
#define _ALBUM_H

#include <libspotify/api.h>
#include <string>

class Album {
public:
  Album(sp_album* _album);
  ~Album() {
    sp_album_release(album);
    if(cover != nullptr) {
      sp_image_release(cover);
    }
  };
  Album(const Album& other) : album(other.album), cover(other.cover) {
    sp_album_add_ref(album);
    sp_image_add_ref(cover);
  };
  std::string name();
  std::string link();
  std::string coverBase64();
private:
  sp_album* album;
  sp_image* cover;
};

#endif