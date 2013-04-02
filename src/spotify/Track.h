#ifndef _TRACK_H
#define _TRACK_H

#include "SpotifyWrapped.h"
#include <libspotify/api.h>
#include <string>
#include "Artist.h"

using namespace v8;

class Track : public SpotifyWrapped<Track> {
  public:
    Track(sp_track* _track, uv_async_t* _handle, std::string _name, Artist* _artist) : SpotifyWrapped(_handle), spotifyTrack(_track), name(_name), artist(_artist) {};

    static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getArtist(Local<String> property, const AccessorInfo& info);

    static void init(Handle<Object> target);
  private:
    sp_track* spotifyTrack;
    std::string name;
    Artist* artist;
};

#endif
