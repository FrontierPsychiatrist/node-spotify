#ifndef _ARTIST_H
#define _ARTIST_H

#include <v8.h>
#include <string>
#include <map>
#include <libspotify/api.h>
#include "SpotifyWrapped.h"

using namespace v8;

class Artist : public SpotifyWrapped<Artist> {
  public:
    Artist(std::string _name, sp_artist* _spArtist) : SpotifyWrapped(0), name(_name), spArtist(_spArtist) {};
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
