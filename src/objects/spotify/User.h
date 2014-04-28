/**
The MIT License (MIT)

Copyright (c) <2014> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#ifndef _USER_H
#define _USER_H

#include "PlaylistContainer.h"
#include "StarredPlaylist.h"

#include <libspotify/api.h>
#include <string>
#include <memory>

class PlaylistContainer;

class User {
friend class NodeUser;
public:
  User(sp_user* user);
  User(const User& other);
  ~User();

  std::string canonicalName();
  std::string displayName();
  std::string link();
  bool isLoaded();
  std::shared_ptr<PlaylistContainer> publishedPlaylists();
  std::shared_ptr<Playlist> starredPlaylist();
private:
  sp_user* user;
};

#endif
