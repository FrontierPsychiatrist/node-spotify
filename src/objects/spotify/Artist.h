#ifndef _ARTIST_H
#define _ARTIST_H

#include <string>
#include <map>
#include <memory>
#include <libspotify/api.h>

class Artist {
public:
  Artist(sp_artist* _artist);
  Artist(const Artist& other) : artist(other.artist) {
    sp_artist_add_ref(artist);
  };
  ~Artist() {
    sp_artist_release(artist);
  };

  std::string name();
  std::string link();
private:
  sp_artist* artist;
};

#endif
