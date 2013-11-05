#ifndef _ARTIST_H
#define _ARTIST_H

#include <string>
#include <vector>
#include <memory>
#include <libspotify/api.h>

#include "Track.h"
#include "Album.h"
#include "../node/V8Callable.h"

class Track;
class Album;

class Artist {
friend class NodeArtist;
friend class ArtistBrowseCallbacks;
public:
  Artist(sp_artist* _artist);
  Artist(const Artist& other);
  ~Artist();

  std::string name();
  std::string link();
  std::vector<std::shared_ptr<Track>> tracks();
  std::vector<std::shared_ptr<Track>> tophitTracks();
  std::vector<std::shared_ptr<Album>> albums();
  std::vector<std::shared_ptr<Artist>> similarArtists();
  std::string biography();
  void browse(sp_artistbrowse_type artistbrowseType);
private:
  sp_artist* artist;
  sp_artistbrowse* artistBrowse;
  V8Callable* nodeObject;
};

#endif