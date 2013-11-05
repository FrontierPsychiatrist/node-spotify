#ifndef _ALBUM_H
#define _ALBUM_H

#include <libspotify/api.h>
#include <string>
#include <vector>
#include <memory>

#include "Track.h"
#include "Artist.h"
#include "../node/V8Callable.h"

class Track;
class Artist;

class Album {
friend class NodeAlbum;
friend class AlbumBrowseCallbacks;
public:
  Album(sp_album* _album);
  ~Album();
  Album(const Album& other);
  std::string name();
  std::string link();
  std::string coverBase64();
  std::vector<std::shared_ptr<Track>> tracks();
  std::string review();
  std::vector<std::string> copyrights();
  std::shared_ptr<Artist> artist();
  void browse();
private:
  sp_album* album;
  sp_image* cover;
  V8Callable* nodeObject;
  sp_albumbrowse* albumBrowse;
};

#endif