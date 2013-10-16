#include "Artist.h"

Artist::Artist(sp_artist* _artist) : artist(_artist) {
  sp_artist_add_ref(artist);
};

std::string Artist::name() {
  std::string name;
  if(sp_artist_is_loaded(artist)) {
    name = std::string(sp_artist_name(artist));
  } else {
    name = "Loading...";
  }
  return name;
}

std::string Artist::link() {
  std::string link;
  if(sp_artist_is_loaded(artist)) {
    sp_link* spLink = sp_link_create_from_artist(artist);
    char linkChar[256];
    sp_link_as_string(spLink, linkChar, 256);
    link = std::string(linkChar);
    sp_link_release(spLink);
  }
  return link;
}