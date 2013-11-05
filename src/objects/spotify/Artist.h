#ifndef _ARTIST_H
#define _ARTIST_H

#include <string>
#include <map>
#include <memory>
#include <libspotify/api.h>

class Artist {
friend class NodeArtist;
public:
  Artist(sp_artist* _artist);
  Artist(const Artist& other);
  ~Artist();

  std::string name();
  std::string link();
  void browse();
private:
  sp_artist* artist;
  sp_artistbrowse* artistBrowse;
};

#endif
