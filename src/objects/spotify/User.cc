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

#include "User.h"
#include "StarredPlaylist.h"
#include "../../Application.h"

extern Application* application;

User::User(sp_user* _user) : user(_user) {
  sp_user_add_ref(user);
};

User::User(const User& other) : user(other.user) {
  sp_user_add_ref(user);
};

User::~User() {
  sp_user_release(user);
}

bool User::isLoaded() {
  return sp_user_is_loaded(user);
}

std::string User::canonicalName() {
  if(sp_user_is_loaded(user)) {
    return std::string(sp_user_canonical_name(user));
  } else {
    return "Loading";
  }
}

std::string User::displayName() {
  if(sp_user_is_loaded(user)) {
    return std::string(sp_user_display_name(user));
  } else {
    return "Loading";
  }
}

std::string User::link() {
  std::string link;
  if(sp_user_is_loaded(user)) {
    char linkChar[256];
    sp_link* spLink = sp_link_create_from_user(user);
    sp_link_as_string(spLink, linkChar, 256);
    link = std::string(linkChar);
    sp_link_release(spLink);
  }
  return link;
}

std::shared_ptr<PlaylistContainer> User::publishedPlaylists() {
  std::shared_ptr<PlaylistContainer> plc;
  if(sp_user_is_loaded(user)) {
    auto spPlc = sp_session_publishedcontainer_for_user_create(application->session, sp_user_canonical_name(user));
    plc = std::make_shared<PlaylistContainer>(spPlc);
  }
  return plc;
}

std::shared_ptr<Playlist> User::starredPlaylist() {
  std::shared_ptr<Playlist> playlist;
  if(sp_user_is_loaded(user)) {
    auto spotifyPlaylist = sp_session_starred_for_user_create(application->session, sp_user_canonical_name(user));
    playlist = std::make_shared<StarredPlaylist>(spotifyPlaylist);
  }
  return playlist;
}