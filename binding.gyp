{
  "targets": [
  {
    "target_name": "spotify",
    "sources": ["src/node-spotify.cc", "src/appkey.c", "src/SpotifyService/SpotifyService.cc",
      "src/spotify/Track.cc", "src/spotify/Artist.cc",
      "src/spotify/Playlist.cc", "src/spotify/PlaylistContainer.cc",
      "src/spotify/Player.cc", "src/spotify/Album.cc",
      "src/SpotifyService/PlaylistCallbacks.cc",
      "src/SpotifyService/SessionCallbacks.cc",
      "src/audio/audio.c"
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
        "ccflags": ["-std=c++11"],
        "defines": ["OS_LINUX"],
        "link_settings" : { "libraries" : ["-lasound"] }
      }]
    ]
  }
  ]
}
