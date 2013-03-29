#ifndef _TRACK_H
#define _TRACK_H

#include "SpotifyWrapped.h"
#include <libspotify/api.h>
#include <string>

using namespace v8;

class Track : public SpotifyWrapped<Track> {
  public:
    Track(sp_track* _track, uv_async_t* _handle, std::string _name ) : SpotifyWrapped(_handle), spotifyTrack(_track), name(_name) {};

    static Handle<Value> getName(Local<String> property, const AccessorInfo& info);

    static void init(Handle<Object> target);

  private:
    sp_track* spotifyTrack;
    std::string name;
};

#endif
