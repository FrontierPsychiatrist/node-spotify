#ifndef _TRACK_H
#define _TRACK_H

#include "SpotifyWrapped.h"
#include <libspotify/api.h>
#include <string>
#include <vector>
#include "Artist.h"

using namespace v8;

class Track : public SpotifyWrapped<Track> {
  friend class Player;
  public:
    Track(sp_track* _track, uv_async_t* _handle, std::string _name, std::vector<Artist*> _artists) : 
      SpotifyWrapped(_handle), spotifyTrack(_track), name(_name), artists(_artists) {};

    static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getArtists(Local<String> property, const AccessorInfo& info);

    static Handle<Value> play(const Arguments& args);    

    static void init(Handle<Object> target);
  private:
    void spotifyPlay();
    sp_track* spotifyTrack;
    std::string name;
    std::vector<Artist*> artists;
};

#endif
