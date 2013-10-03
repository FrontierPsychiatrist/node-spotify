#ifndef _TRACK_H
#define _TRACK_H

#include <libspotify/api.h>
#include <string>
#include <vector>
#include <memory>

#include "Artist.h"
#include "Album.h"

#include "../../Application.h"

extern Application* application;

class Track {
friend class NodePlayer;
friend class NodeTrack;
public:
  Track(sp_track* _track) : track(_track) {
    sp_track_add_ref(track);
    starred = sp_track_is_starred(application->session, track);
    name = std::string(sp_track_name(track));
    popularity = sp_track_popularity(track);
    duration = sp_track_duration(track);
    for(int i = 0; i < sp_track_num_artists(track); i++) {
      sp_artist* spArtist = sp_track_artist(track, i);
      std::shared_ptr<Artist> artist = Artist::fromCache(spArtist);
      artists.push_back(artist);
    }
    sp_album* spAlbum = sp_track_album(track);
    album = Album::fromCache(spAlbum);
  };
  Track(const Track& other) : track(other.track), name(other.name), artists(other.artists),
      album(other.album), duration(other.duration), starred(other.starred), popularity(other.popularity) {
      sp_track_add_ref(track);
    };
  ~Track() {
    sp_track_release(track);
  };
  void setStarred(bool starred);
private:
  sp_track* track;
  std::string name;
  std::vector<std::shared_ptr<Artist>> artists;
  std::shared_ptr<Album> album;
  int duration;
  bool starred;
  int popularity;
};

#endif
