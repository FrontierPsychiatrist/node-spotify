#ifndef _PLAYLIST_CONTAINER_CALLBACKS_H
#define _PLAYLIST_CONTAINER_CALLBACKS_H

#include <node.h>

#include <libspotify/api.h>
#include <nan.h>

class PlaylistContainerCallbacksHolder {
private:
  sp_playlistcontainer* playlistContainer;
  sp_playlistcontainer_callbacks* playlistContainerCallbacks;
  node::ObjectWrap* userdata;
public:
  PlaylistContainerCallbacksHolder(sp_playlistcontainer* pc, node::ObjectWrap* userdata);
  ~PlaylistContainerCallbacksHolder();

  //libspotify callback functions
  static void playlistAdded(sp_playlistcontainer* pc, sp_playlist* spPlaylist, int position, void* userdata);
  static void playlistRemoved(sp_playlistcontainer *pc, sp_playlist *playlist, int position, void *userdata);
  static void playlistMoved(sp_playlistcontainer *pc, sp_playlist *playlist, int position, int new_position, void *userdata);

  Nan::Callback playlistAddedCallback;
  Nan::Callback playlistRemovedCallback;
  Nan::Callback playlistMovedCallback;

  void setCallbacks();
  void unsetCallbacks();
};

#endif