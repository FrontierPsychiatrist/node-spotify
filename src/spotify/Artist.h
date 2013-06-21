#ifndef _ARTIST_H
#define _ARTIST_H

#include <string>
#include <map>
#include <libspotify/api.h>
#include "SpotifyWrapped.h"

using namespace v8;

class Artist : public SpotifyWrapped<Artist> {
  public:
    Artist(sp_artist* _spArtist) : SpotifyWrapped(), spArtist(_spArtist) {
      sp_artist_add_ref(spArtist);
      name = std::string(sp_artist_name(spArtist));
    };

    static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
    static void init(Handle<Object> target);

    static Artist* getArtist(sp_artist* _spArtist);
    static void putArtist(Artist* artist);
  private:
    static std::map<sp_artist*, Artist*> artistCache;
    std::string name;
    sp_artist* spArtist;
};

#endif
