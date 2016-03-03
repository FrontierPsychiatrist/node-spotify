#include "NodeSpotify.h"
#include "../../audio/AudioHandler.h"
#include "../../audio/NativeAudioHandler.h"
#include "../../audio/NodeAudioHandler.h"
#include "../../Application.h"
#include "../../exceptions.h"
#include "../../callbacks/SessionCallbacks.h"
#include "../spotify/SpotifyOptions.h"
#include "NodePlaylist.h"
#include "NodePlaylistFolder.h"
#include "NodePlaylistContainer.h"
#include "NodePlayer.h"
#include "NodeArtist.h"
#include "NodeAlbum.h"
#include "NodeTrack.h"
#include "NodeUser.h"
#include "../../utils/V8Utils.h"

extern Application* application;

NodeSpotify::NodeSpotify(Handle<Object> options) {
  /*
   * Important note: The session callbacks must be initialized before
   * the sp_session is started. The uv timer and async handle must be set up
   * as they will be used as soon as the sp_session is created, which happens in the
   * NodeSpotify ctor.
   */
  SessionCallbacks::init();

  SpotifyOptions _options;
  NanScope();
  Handle<String> settingsFolderKey = NanNew<String>("settingsFolder");
  Handle<String> cacheFolderKey = NanNew<String>("cacheFolder");
  Handle<String> traceFileKey = NanNew<String>("traceFile");
  Handle<String> appkeyFileKey = NanNew<String>("appkeyFile");
  if(options->Has(settingsFolderKey)) {
    String::Utf8Value settingsFolderValue(options->Get(settingsFolderKey)->ToString());
    _options.settingsFolder = *settingsFolderValue;
  } else {
    _options.settingsFolder = "settings";
  }
  if(options->Has(cacheFolderKey)) {
    String::Utf8Value cacheFolderValue(options->Get(cacheFolderKey)->ToString());
    _options.cacheFolder = *cacheFolderValue;
  } else {
    _options.cacheFolder = "cache";
  }
  if(options->Has(traceFileKey)) {
    String::Utf8Value traceFileValue(options->Get(traceFileKey)->ToString());
    _options.traceFile = *traceFileValue;
  }
  if(options->Has(appkeyFileKey)) {
    String::Utf8Value appkeyFileValue(options->Get(appkeyFileKey)->ToString());
    _options.appkeyFile = *appkeyFileValue;
  }
  spotify = std::unique_ptr<Spotify>(new Spotify(_options));
}

NodeSpotify::~NodeSpotify() {

}

NAN_METHOD(NodeSpotify::createFromLink) {
  NanScope();
  Handle<Value> out;
  String::Utf8Value linkToParse(args[0]->ToString());
  sp_link* parsedLink = sp_link_create_from_string(*linkToParse);
  if(parsedLink != nullptr) {
    sp_linktype linkType = sp_link_type(parsedLink);
    switch(linkType) {
      case SP_LINKTYPE_TRACK:
      {
        sp_track* track = sp_link_as_track(parsedLink);
        NodeTrack* nodeTrack = new NodeTrack(std::make_shared<Track>(track));
        out = nodeTrack->createInstance();
        break;
      }
      case SP_LINKTYPE_ALBUM:
      {
        sp_album* album = sp_link_as_album(parsedLink);
        NodeAlbum* nodeAlbum = new NodeAlbum(std::unique_ptr<Album>(new Album(album)));
        out = nodeAlbum->createInstance();
        break;
      }
      case SP_LINKTYPE_ARTIST:
      {
        sp_artist* artist = sp_link_as_artist(parsedLink);
        NodeArtist* nodeArtist = new NodeArtist(std::unique_ptr<Artist>(new Artist(artist)));
        out = nodeArtist->createInstance();
        break;
      }
      case SP_LINKTYPE_PROFILE:
      {
        sp_user* user = sp_link_as_user(parsedLink);
        NodeUser* nodeUser = new NodeUser(std::unique_ptr<User>(new User(user)));
        out = nodeUser->createInstance();
        break;
      }
      case SP_LINKTYPE_PLAYLIST:
      {
        sp_playlist* spPlaylist = sp_playlist_create(application->session, parsedLink);
        auto playlist = Playlist::fromCache(spPlaylist);
        NodePlaylist* nodePlaylist = new NodePlaylist(playlist);
        out = nodePlaylist->createInstance();
        break;
      }
      case SP_LINKTYPE_LOCALTRACK:
      {
        sp_track* track = sp_link_as_track(parsedLink);
        NodeTrack* nodeTrack = new NodeTrack(std::make_shared<Track>(track));
        out = nodeTrack->createInstance();
        break;
      }
      default:
        out = NanUndefined();
    }
    sp_link_release(parsedLink);
  } else {
    out = NanUndefined();
  }
  NanReturnValue(out);
}

NAN_METHOD(NodeSpotify::login) {
  NanScope();
  NodeSpotify* nodeSpotify = node::ObjectWrap::Unwrap<NodeSpotify>(args.This());
  String::Utf8Value v8User(args[0]->ToString());
  String::Utf8Value v8Password(args[1]->ToString());
  bool rememberMe = args[2]->ToBoolean()->Value();
  bool withRemembered = args[3]->ToBoolean()->Value();
  std::string user(*v8User);
  std::string password(*v8Password);
  nodeSpotify->spotify->login(user, password, rememberMe, withRemembered);
  NanReturnUndefined();
}

NAN_METHOD(NodeSpotify::logout) {
  NanScope();
  if(args.Length() > 0) {
    SessionCallbacks::logoutCallback = std::unique_ptr<NanCallback>(new NanCallback(args[0].As<Function>()));
  }
  NodeSpotify* nodeSpotify = node::ObjectWrap::Unwrap<NodeSpotify>(args.This());
  nodeSpotify->spotify->logout();
  NanReturnUndefined();
}

NAN_GETTER(NodeSpotify::getPlaylistContainer) {
  NanScope();
  NodePlaylistContainer* nodePlaylistContainer = new NodePlaylistContainer(application->playlistContainer);
  NanReturnValue(nodePlaylistContainer->createInstance());
}

NAN_GETTER(NodeSpotify::getRememberedUser) {
  NanScope();
  NodeSpotify* nodeSpotify = node::ObjectWrap::Unwrap<NodeSpotify>(args.This());
  NanReturnValue(NanNew<String>(nodeSpotify->spotify->rememberedUser().c_str()));
}

NAN_GETTER(NodeSpotify::getSessionUser) {
  NanScope();
  NodeSpotify* nodeSpotify = node::ObjectWrap::Unwrap<NodeSpotify>(args.This());
  NodeUser* nodeUser = new NodeUser(std::move(nodeSpotify->spotify->sessionUser()));
  NanReturnValue(nodeUser->createInstance());
}

NAN_GETTER(NodeSpotify::getSessionUserCountry) {
  NanScope();
  NodeSpotify* nodeSpotify = node::ObjectWrap::Unwrap<NodeSpotify>(args.This());
  NanReturnValue(NanNew<String>(nodeSpotify->spotify->sessionUserCountry().c_str()));
}

NAN_GETTER(NodeSpotify::getConstants) {
  NanScope();
  Local<Object> constants = NanNew<Object>();
  constants->Set(NanNew<String>("ARTISTBROWSE_FULL"), NanNew<Number>(SP_ARTISTBROWSE_FULL));
  constants->Set(NanNew<String>("ARTISTBROWSE_NO_TRACKS"), NanNew<Number>(SP_ARTISTBROWSE_NO_TRACKS));
  constants->Set(NanNew<String>("ARTISTBROWSE_NO_ALBUMS"), NanNew<Number>(SP_ARTISTBROWSE_NO_ALBUMS));

  constants->Set(NanNew<String>("PLAYLIST_TYPE_PLAYLIST"), NanNew<Number>(SP_PLAYLIST_TYPE_PLAYLIST));
  constants->Set(NanNew<String>("PLAYLIST_TYPE_START_FOLDER"), NanNew<Number>(SP_PLAYLIST_TYPE_START_FOLDER));
  constants->Set(NanNew<String>("PLAYLIST_TYPE_END_FOLDER"), NanNew<Number>(SP_PLAYLIST_TYPE_END_FOLDER));
  constants->Set(NanNew<String>("PLAYLIST_TYPE_PLACEHOLDER"), NanNew<Number>(SP_PLAYLIST_TYPE_PLACEHOLDER));


  constants->Set(NanNew<String>("SP_TRACK_AVAILABILITY_UNAVAILABLE"), NanNew<Number>(SP_TRACK_AVAILABILITY_UNAVAILABLE));
  constants->Set(NanNew<String>("SP_TRACK_AVAILABILITY_AVAILABLE"), NanNew<Number>(SP_TRACK_AVAILABILITY_AVAILABLE));
  constants->Set(NanNew<String>("SP_TRACK_AVAILABILITY_NOT_STREAMABLE"), NanNew<Number>(SP_TRACK_AVAILABILITY_NOT_STREAMABLE));
  constants->Set(NanNew<String>("SP_TRACK_AVAILABILITY_BANNED_BY_ARTIST"), NanNew<Number>(SP_TRACK_AVAILABILITY_BANNED_BY_ARTIST));

  NanReturnValue(constants);
}

#ifdef NODE_SPOTIFY_NATIVE_SOUND
NAN_METHOD(NodeSpotify::useNativeAudio) {
  NanScope();
  //Since the old audio handler has to be deleted first, do an empty reset.
  application->audioHandler.reset();
  application->audioHandler = std::unique_ptr<AudioHandler>(new NativeAudioHandler());
  NanReturnUndefined();
}
#endif

NAN_METHOD(NodeSpotify::useNodejsAudio) {
  NanScope();
  if(args.Length() < 1) {
    return NanThrowError("useNodjsAudio needs a function as its first argument.");
  }
  //Since the old audio handler has to be deleted first, do an empty reset.
  application->audioHandler.reset();
  auto callback = std::unique_ptr<NanCallback>(new NanCallback(args[0].As<Function>()));
  application->audioHandler = std::unique_ptr<AudioHandler>(new NodeAudioHandler(std::move(callback)));

  Handle<Function> needMoreDataSetter = NanNew<FunctionTemplate>(NodeAudioHandler::setNeedMoreData)->GetFunction();
  NanReturnValue(needMoreDataSetter);
}

NAN_METHOD(NodeSpotify::on) {
  NanScope();
  if(args.Length() < 1 || !args[0]->IsObject()) {
    return NanThrowError("on needs an object as its first argument.");
  }
  Handle<Object> callbacks = args[0]->ToObject();
  Handle<String> metadataUpdatedKey = NanNew<String>("metadataUpdated");
  Handle<String> readyKey = NanNew<String>("ready");
  Handle<String> logoutKey = NanNew<String>("logout");
  Handle<String> playTokenLostKey = NanNew<String>("playTokenLost");
  SessionCallbacks::metadataUpdatedCallback = V8Utils::getFunctionFromObject(callbacks, metadataUpdatedKey);
  SessionCallbacks::loginCallback = V8Utils::getFunctionFromObject(callbacks, readyKey);
  SessionCallbacks::logoutCallback = V8Utils::getFunctionFromObject(callbacks, logoutKey);
  SessionCallbacks::playTokenLostCallback = V8Utils::getFunctionFromObject(callbacks, playTokenLostKey);
  NanReturnUndefined();
}

void NodeSpotify::init() {
  NanScope();
  Handle<FunctionTemplate> constructorTemplate = NodeWrapped::init("Spotify");
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "login", login);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "logout", logout);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "createFromLink", createFromLink);
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "on", on);
#ifdef NODE_SPOTIFY_NATIVE_SOUND
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "useNativeAudio", useNativeAudio);
#endif
  NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "useNodejsAudio", useNodejsAudio);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("rememberedUser"), getRememberedUser);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("sessionUser"), getSessionUser);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("country"), getSessionUserCountry);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("playlistContainer"), getPlaylistContainer);
  constructorTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("constants"), getConstants);

  NanAssignPersistent(NodeSpotify::constructorTemplate, constructorTemplate);
}
