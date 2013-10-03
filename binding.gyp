{
  "targets": [
  {
    "target_name": "spotify",
    "sources": [
      "src/node-spotify.cc", "src/appkey.c", "src/audio/audio.c",
      "src/SpotifyService/SpotifyService.cc",
      "src/SpotifyService/PlaylistCallbacks.cc",
      "src/SpotifyService/SessionCallbacks.cc",

      "src/objects/spotify/Track.cc", "src/objects/spotify/Artist.cc",
      "src/objects/spotify/Playlist.cc", "src/objects/spotify/PlaylistContainer.cc",
      "src/objects/spotify/Album.cc",

      "src/objects/node/NodeTrack.cc", "src/objects/node/NodeArtist.cc",
      "src/objects/node/NodePlaylist.cc", "src/objects/node/NodeAlbum.cc",
      "src/objects/node/NodePlayer.cc"
    ],
    "link_settings" : {
      "libraries": ["-lspotify"]
    },
    "actions": [ {
      "action_name": "generateEventsHeader",
      "inputs": ["frontend/app/scripts/services/events.js"],
      "outputs": ["src/events.h"],
      "action": ["node", "preprocessor.js"],
      "message": "Generating C++ events headerfile"
      }
    ],
    "conditions": [
      ["OS=='mac'", {
        "xcode_settings": {
          "OTHER_CPLUSPLUSFLAGS" : ["-std=c++11", "-stdlib=libc++"],
          "MACOSX_DEPLOYMENT_TARGET" : "10.8"
        },
        "sources": ["src/audio/openal-audio.c"],
        "defines": ["OS_OSX"],
        "link_settings" : { "libraries" : ["-framework", "OpenAL"] }
      }],

      ["OS=='linux'", {
        "sources": ["src/audio/alsa-audio.c"],
        "cflags": ["-I/usr/include/alsa"],
        "cflags_cc": ["-std=c++11"],
        "defines": ["OS_LINUX"],
        "link_settings" : { "libraries" : ["-lasound"] }
      }]
    ]
  }
  ]
}
