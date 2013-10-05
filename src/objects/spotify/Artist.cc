#include "Artist.h"

std::map<sp_artist*, std::shared_ptr<Artist>> Artist::cache;

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