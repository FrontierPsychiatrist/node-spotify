/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

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

#ifndef _SPOTIFY_H
#define _SPOTIFY_H

#include "SpotifyOptions.h"
#include "User.h"
#include "Playlist.h"

#include <libspotify/api.h>
#include <string>
#include <memory>

class Spotify {
public:
  Spotify(SpotifyOptions options);
  ~Spotify() {};
  void login(std::string user, std::string password, bool remeberedUser, bool withRemembered);
  void logout();
  std::string rememberedUser();
  std::shared_ptr<User> sessionUser();
  std::shared_ptr<Playlist> starredPlaylist();
private:
  sp_session* session;
  sp_session* createSession(SpotifyOptions options);
};

#endif