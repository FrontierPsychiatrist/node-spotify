#include "Artist.h"

std::map<sp_artist*, std::shared_ptr<Artist>> Artist::cache;

std::shared_ptr<Artist> Artist::fromCache(sp_artist* artist) {
  auto it = Artist::cache.find(artist);
  if(it != Artist::cache.end()) {
    return it->second;
  } else {
    std::shared_ptr<Artist> artistPointer(new Artist(artist));
    Artist::cache[artist] = artistPointer;
    return artistPointer;
  }
}