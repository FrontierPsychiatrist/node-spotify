#ifndef _ALBUM_H
#define _ALBUM_H

#include <libspotify/api.h>
#include <string>

#include "SpotifyWrapped.h"

using namespace v8;

void imageLoadedCallback(sp_image* image, void* userdata);

class Album : public SpotifyWrapped<Album> {
  friend class Player;
  public:
    Album(sp_album* _album) : SpotifyWrapped(), spAlbum(_album), coverBase64(0) {
      sp_album_add_ref(spAlbum);
      name = std::string(sp_album_name(spAlbum));
      const byte* coverId = sp_album_cover(spAlbum, SP_IMAGE_SIZE_NORMAL);
  
      if(coverId != 0) {
        sp_image* image = sp_image_create(spotifyService->spotifySession, coverId);
        if(sp_image_is_loaded(image)) {
          processImage(image);
          sp_image_release(image);
        } else {
          sp_image_add_load_callback(image, &imageLoadedCallback, this);
        }
      }
    };

    static void init(Handle<Object> target);
    static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getCoverBase64(const Arguments& args);
    static Album* getAlbum(sp_album* _spAlbum);
    static void putAlbum(Album* album);

    void processImage(sp_image* image);
  private:
    static std::map<sp_album*, Album*> albumCache;
    std::string name;
    sp_album* spAlbum;
    char* coverBase64;
};

#endif