#ifndef _TRACK_H
#define _TRACK_H

#include "SpotifyWrapped.h"
#include <libspotify/api.h>
#include <string>

using namespace v8;

class Track : public SpotifyWrapped {
  public:
    Track(sp_track* _track, uv_async_t* _handle, std::string _name ) : SpotifyWrapped(_handle), spotifyTrack(_track), name(_name) {};

    static Handle<Value> getName(Local<String> property, const AccessorInfo& info);

    static void init(Handle<Object> target);

  protected:
    Persistent<Function>& getConstructor() { return constructor; };
  private:
    static v8::Persistent<v8::Function> constructor;  
    sp_track* spotifyTrack;
    std::string name;
};

#endif
