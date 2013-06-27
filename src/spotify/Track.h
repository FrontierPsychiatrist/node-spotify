#ifndef _TRACK_H
#define _TRACK_H

#include <libspotify/api.h>
#include <string>
#include <vector>

#include "SpotifyWrapped.h"
#include "Artist.h"
#include "Album.h"
#include "../SpotifyService/SpotifyService.h"

extern SpotifyService* spotifyService;

using namespace v8;

class Track : public SpotifyWrapped<Track> {
  friend class Player;
  public:
    Track(sp_track* _track) : SpotifyWrapped(), spotifyTrack(_track) {
      sp_track_add_ref(spotifyTrack);
      starred = sp_track_is_starred(spotifyService->spotifySession, spotifyTrack);
      name = std::string(sp_track_name(spotifyTrack));
      duration = sp_track_duration(spotifyTrack);
      for(int i = 0; i < sp_track_num_artists(spotifyTrack); i++) {
        sp_artist* spArtist = sp_track_artist(spotifyTrack, i);
        Artist* artist = Artist::getArtist(spArtist);
        if(artist == 0) {
          artist = new Artist(spArtist);
          Artist::putArtist(artist);
        }
        artists.push_back(artist);
      }
      
      sp_album* spAlbum = sp_track_album(spotifyTrack);
      album = Album::getAlbum(spAlbum);
      if(album == 0) {
        album = new Album(spAlbum);
        Album::putAlbum(album);
      }

    };

    static Handle<Value> getName(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getArtists(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getDuration(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getAlbum(Local<String> property, const AccessorInfo& info);
    static Handle<Value> getStarred(Local<String> property, const AccessorInfo& info);
    static void setStarred(Local<String> property, Local<Value> value, const AccessorInfo& info);

    static void init(Handle<Object> target);
  private:
    sp_track* spotifyTrack;
    std::string name;
    std::vector<Artist*> artists;
    Album* album;
    int duration;
    bool starred;
};

#endif
