#ifndef _ARTIST_H
#define _ARTIST_H

#include <string>
#include <map>
#include <memory>
#include <libspotify/api.h>

class Artist {
friend class NodeArtist;
public:
  Artist(sp_artist* _artist);
  Artist(const Artist& other) : name(other.name), link(other.link), artist(other.artist) {
    sp_artist_add_ref(artist);
  };
  ~Artist() {
    sp_artist_release(artist);
  };
  static std::shared_ptr<Artist> fromCache(sp_artist* artist);
  static void clearCache();
private:
  std::string name;
  std::string link;
  sp_artist* artist;
  static std::map<sp_artist*, std::shared_ptr<Artist>> cache;
};

#endif
