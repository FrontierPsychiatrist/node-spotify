#ifndef _TRACK_H
#define _TRACK_H

#include <libspotify/api.h>
#include <string>
#include <vector>
#include <memory>

#include "Artist.h"
#include "Album.h"

class Track {
friend class NodePlayer;
friend class NodeTrack;
public:
  Track(sp_track* _track);
  Track(const Track& other) : track(other.track), name(other.name), link(other.link), artists(other.artists),
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
  std::string link;
  std::vector<std::shared_ptr<Artist>> artists;
  std::shared_ptr<Album> album;
  int duration;
  bool starred;
  int popularity;
};

#endif
