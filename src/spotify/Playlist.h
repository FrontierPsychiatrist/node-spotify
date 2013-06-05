#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <node.h>
#include <string>
#include <vector>
#include <libspotify/api.h>
#include <pthread.h>

#include "SpotifyWrapped.h"
#include "Track.h"
#include "../Callback.h"

using namespace v8;

class Playlist : public SpotifyWrapped<Playlist> {
  friend class Callback<Playlist>;
  public:
    Playlist(sp_playlist* _playlist, uv_async_t* _handle, int _id) : SpotifyWrapped(_handle), id(_id), playlist(_playlist), tracksLoaded(0) {};
    std::string name;
	
    std::vector<Track*> getTracks();

    //Method visible to nodeJS
    static void setName(Local<String> property, Local<Value> value, const AccessorInfo& info);
    static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getId(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getTracks(const Arguments& args);

    static void init(Handle<Object> target);
  private:
    int id;
    std::vector<Track*> tracks;
    sp_playlist* playlist;
    bool tracksLoaded;
    void loadTracks();
};

#endif
