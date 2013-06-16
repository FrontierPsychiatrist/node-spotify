#ifndef _TRACK_H
#define _TRACK_H

#include "SpotifyWrapped.h"
#include <libspotify/api.h>
#include <string>
#include <vector>
#include "Artist.h"
#include "Album.h"

using namespace v8;

class Track : public SpotifyWrapped<Track> {
  friend class Player;
  public:
    Track(sp_track* _track, std::string _name, std::vector<Artist*> _artists, Album* _album) : 
      SpotifyWrapped(), spotifyTrack(_track), name(_name), artists(_artists), album(_album) {};

    static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getArtists(Local<String> property, const AccessorInfo& info);

    static Handle<Value> getAlbum(Local<String> property, const AccessorInfo& info);

    static void init(Handle<Object> target);
  private:
    sp_track* spotifyTrack;
    std::string name;
    std::vector<Artist*> artists;
    Album* album;
};

#endif
