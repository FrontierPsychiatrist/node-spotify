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
  public:
    Playlist(sp_playlist* _playlist, int _id) : SpotifyWrapped(), id(_id), playlist(_playlist), tracksLoaded(0) {
      sp_playlist_add_ref(playlist);
      if(!sp_playlist_is_loaded(playlist)) {
        name = std::string("Loading...");
      } else {
        name = std::string(sp_playlist_name(playlist));  
      }
    };
	
    std::vector<Track*> getTracks();
    void setName(std::string _name);

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
    std::string name;
    bool tracksLoaded;
    void loadTracks();
};

#endif
