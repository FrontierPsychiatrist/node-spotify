#ifndef _ALBUM_H
#define _ALBUM_H

#include <libspotify/api.h>
#include <string>
#include <map>
#include <memory>

class Album {
friend class NodeAlbum;
public:
  Album(sp_album* _album);
  ~Album() {
    sp_album_release(album);
  };
  Album(const Album& other) : name(other.name), link(other.link), album(other.album), coverBase64(other.coverBase64) {
    sp_album_add_ref(album);
  };
  void processImage(sp_image* image);
  static std::shared_ptr<Album> fromCache(sp_album* album);
  static void clearCache();
private:
  std::string name;
  std::string link;
  sp_album* album;
  char* coverBase64;
  static std::map<sp_album*, std::shared_ptr<Album>> cache;
};

#endif