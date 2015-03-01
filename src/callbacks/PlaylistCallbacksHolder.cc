#include "PlaylistCallbacksHolder.h"
#include "../objects/node/NodeTrackExtended.h"
#include "../objects/node/NodeUser.h"
#include "../objects/spotify/TrackExtended.h"
#include "../objects/spotify/Playlist.h"
#include "../objects/spotify/User.h"

#include <memory>

PlaylistCallbacksHolder::PlaylistCallbacksHolder(node::ObjectWrap* _userdata, sp_playlist* _playlist) : userdata(_userdata), playlist(_playlist) {
  playlistCallbacks = new sp_playlist_callbacks();
}

PlaylistCallbacksHolder::~PlaylistCallbacksHolder() {
  sp_playlist_remove_callbacks(playlist, playlistCallbacks, this);
  delete playlistCallbacks;
}

void PlaylistCallbacksHolder::call(std::unique_ptr<NanCallback>& callback, std::initializer_list<Handle<Value>> args) {
  unsigned int argc = args.size();
  Handle<Value>* argv = const_cast<Handle<Value>*>(args.begin());
  callback->Call(argc, argv);
}

void PlaylistCallbacksHolder::playlistRenamed(sp_playlist* spPlaylist, void* userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  holder->call(holder->playlistRenamedCallback, { NanUndefined(), NanObjectWrapHandle(holder->userdata) });
}

void PlaylistCallbacksHolder::tracksAdded(sp_playlist* spPlaylist, sp_track *const *tracks, int num_tracks, int position, void *userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Handle<Array> nodeTracks = NanNew<Array>(num_tracks);
  for(int i = 0; i < num_tracks; i++) {
    NodeTrack* nodeTrackExtended = new NodeTrackExtended(std::make_shared<TrackExtended>(tracks[i], spPlaylist, position + i));
    nodeTracks->Set(NanNew<Number>(i), nodeTrackExtended->createInstance());
  }
  holder->call(holder->tracksAddedCallback, { NanUndefined(), NanObjectWrapHandle(holder->userdata), nodeTracks, NanNew<Number>(position) });
}

void PlaylistCallbacksHolder::tracksMoved(sp_playlist* spPlaylist, const int* tracks, int num_tracks, int new_position, void *userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Handle<Array> movedTrackIndices = NanNew<Array>(num_tracks);
  for(int i = 0; i < num_tracks; i++) {
    movedTrackIndices->Set(NanNew<Number>(i), NanNew<Number>(tracks[i]));
  }
  holder->call(holder->tracksMovedCallback, { NanUndefined(), NanObjectWrapHandle(holder->userdata), movedTrackIndices, NanNew<Number>(new_position) });
}

void PlaylistCallbacksHolder::tracksRemoved(sp_playlist* spPlaylist, const int *tracks, int num_tracks, void *userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  Handle<Array> removedTrackIndexes = NanNew<Array>(num_tracks);
  for(int i = 0; i < num_tracks; i++) {
    removedTrackIndexes->Set(NanNew<Number>(i), NanNew<Number>(tracks[i]));
  }
  holder->call(holder->tracksRemovedCallback, { NanUndefined(), NanObjectWrapHandle(holder->userdata), removedTrackIndexes });
}

void PlaylistCallbacksHolder::trackCreatedChanged(sp_playlist* spPlaylist, int position, sp_user* spUser, int when, void* userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  double date = (double)when * 1000;
  NodeUser* nodeUser = new NodeUser(std::unique_ptr<User>(new User(spUser)));
  holder->call(holder->trackCreatedChangedCallback, { NanUndefined(), NanObjectWrapHandle(holder->userdata), NanNew<Integer>(position), NanObjectWrapHandle(nodeUser), NanNew<Date>(date) });
}

void PlaylistCallbacksHolder::trackSeenChanged(sp_playlist* spPlaylist, int position, bool seen, void* userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  holder->call(holder->trackSeenChangedCallback, { NanUndefined(), NanObjectWrapHandle(holder->userdata), NanNew<Integer>(position), NanNew<Boolean>(seen) });
}

void PlaylistCallbacksHolder::trackMessageChanged(sp_playlist* spPlaylist, int position, const char* message, void* userdata) {
  auto holder = static_cast<PlaylistCallbacksHolder*>(userdata);
  holder->call(holder->trackMessageChangedCallback, { NanUndefined(), NanObjectWrapHandle(holder->userdata), NanNew<Integer>(position), NanNew<String>(message) });
}

void PlaylistCallbacksHolder::setCallbacks() {
  sp_playlist_remove_callbacks(playlist, playlistCallbacks, this);
  
  if(playlistRenamedCallback && !playlistRenamedCallback->IsEmpty()) {
    playlistCallbacks->playlist_renamed = &PlaylistCallbacksHolder::playlistRenamed;  
  }
  if(tracksAddedCallback && !tracksAddedCallback->IsEmpty()) {
    playlistCallbacks->tracks_added = &PlaylistCallbacksHolder::tracksAdded;  
  }
  if(tracksMovedCallback && !tracksMovedCallback->IsEmpty()) {
    playlistCallbacks->tracks_moved = &PlaylistCallbacksHolder::tracksMoved;  
  }
  if(tracksRemovedCallback && !tracksRemovedCallback->IsEmpty()) {
    playlistCallbacks->tracks_removed = &PlaylistCallbacksHolder::tracksRemoved;  
  }
  if(trackCreatedChangedCallback && !trackCreatedChangedCallback->IsEmpty()) {
    playlistCallbacks->track_created_changed = &PlaylistCallbacksHolder::trackCreatedChanged;
  }
  if(trackSeenChangedCallback && !trackSeenChangedCallback->IsEmpty()) {
    playlistCallbacks->track_seen_changed = &PlaylistCallbacksHolder::trackSeenChanged;
  }
  if(trackMessageChangedCallback && !trackMessageChangedCallback->IsEmpty()) {
    playlistCallbacks->track_message_changed = &PlaylistCallbacksHolder::trackMessageChanged;
  }
  sp_playlist_add_callbacks(playlist, playlistCallbacks, this);
}

void PlaylistCallbacksHolder::unsetCallbacks() {
  sp_playlist_remove_callbacks(playlist, playlistCallbacks, this);
}
