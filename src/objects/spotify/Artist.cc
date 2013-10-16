#include "Artist.h"

std::map<sp_artist*, std::shared_ptr<Artist>> Artist::cache;

Artist::Artist(sp_artist* _artist) : artist(_artist) {
  sp_artist_add_ref(artist);
  name = std::string(sp_artist_name(artist));

  sp_link* spLink = sp_link_create_from_artist(artist);
  char linkChar[256];
  sp_link_as_string(spLink, linkChar, 256);
  link = std::string(linkChar);
  sp_link_release(spLink);
};

void Artist::clearCache() {
  Artist::cache.clear();
}

std::shared_ptr<Artist> Artist::fromCache(sp_artist* artist) {
  auto it = Artist::cache.find(artist);
  if(it != Artist::cache.end()) {
    return it->second;
  } else {
    auto artistPointer = std::make_shared<Artist>(artist);
    Artist::cache[artist] = artistPointer;
    return artistPointer;
  }
}