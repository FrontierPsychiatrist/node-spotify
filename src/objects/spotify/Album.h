#ifndef _ALBUM_H
#define _ALBUM_H

#include <libspotify/api.h>
#include <string>

#include "../node/V8Callable.h"

class Album {
friend class NodeAlbum;
friend class AlbumBrowseCallbacks;
public:
  Album(sp_album* _album);
  ~Album() {
    sp_album_release(album);
    if(cover != nullptr) {
      sp_image_release(cover);
    }
    if(albumBrowse != nullptr) {
      sp_albumbrowse_release(albumBrowse);
    }
  };
  Album(const Album& other) : album(other.album), cover(other.cover) {
    sp_album_add_ref(album);
    if(cover != nullptr) {
      sp_image_add_ref(cover);
    }
    if(albumBrowse != nullptr) {
      sp_albumbrowse_add_ref(albumBrowse);
    }
  };
  std::string name();
  std::string link();
  std::string coverBase64();
  void browse();
private:
  sp_album* album;
  sp_image* cover;
  V8Callable* nodeObject;
  sp_albumbrowse* albumBrowse;
};

#endif