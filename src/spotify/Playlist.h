#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <node.h>
#include <string>
#include <vector>
#include <libspotify/api.h>
#include <pthread.h>

#include "SpotifyWrapped.h"
#include "Track.h"
#include "../utils/ImageUtils.h"

#include <iostream>

using namespace v8;

class Playlist : public SpotifyWrapped<Playlist> {
  friend class PlaylistContainer;
  friend class PlaylistCallbacks;
  public:
    Playlist(sp_playlist* _playlist, int _id) : SpotifyWrapped(), id(_id), playlist(_playlist), tracksLoaded(false), playlistImageBase64(0) {
      sp_playlist_add_ref(playlist);
      if(!sp_playlist_is_loaded(playlist)) {
        name = std::string("Loading...");
      } else {
        name = std::string(sp_playlist_name(playlist));  
      }

      byte imageId[20];
      bool hasImage = sp_playlist_get_image(playlist, imageId);
      if(hasImage) {
        sp_image* image = sp_image_create(spotifyService->spotifySession, imageId);
        if(sp_image_is_loaded(image)) {
          playlistImageBase64 = ImageUtils::convertImageToBase64(image);
          sp_image_release(image);
        } else {
          sp_image_add_load_callback(image, &ImageUtils::imageLoadedCallback, &this->playlistImageBase64);
        }
      }
    };
	
    std::vector<Track*> getTracks();
    void setName(std::string _name);

    //Method visible to nodeJS
    static void setName(Local<String> property, Local<Value> value, const AccessorInfo& info);
    static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getId(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getTracks(const Arguments& args);
    static Handle<Value> getImageBase64(const Arguments& args);

    static void init(Handle<Object> target);
  private:
    int id;
    std::vector<Track*> tracks;
    sp_playlist* playlist;
    std::string name;
    bool tracksLoaded;
    void loadTracks();
    char* playlistImageBase64;
};

#endif
