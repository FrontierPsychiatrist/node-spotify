{
	"targets": [
	{
		"target_name": "spotify",
		"sources": ["src/spotify.cc", "src/appkey.c", "src/SpotifyService/SpotifyService.cc",
			"src/spotify/Track.cc", "src/spotify/Artist.cc",
			"src/spotify/Playlist.cc", "src/spotify/PlaylistContainer.cc",
			"src/SpotifyService/PlaylistCallbacks.cc",
			"src/SpotifyService/SessionCallbacks.cc",
      "src/SpotifyService/alsa-audio.c", "src/SpotifyService/audio.c"
		],
		"link_settings" : {
			"libraries": ["-lspotify"]
		},
		"cflags": ["-I/usr/include/alsa"],
		"actions": [ {
			"action_name": "generateEventsHeader",
			"inputs": ["frontend/js/events.js"],
			"outputs": ["src/events.h"],
			"action": ["node", "preprocessor.js"],
			"message": "Generating C++ events headerfile"
			}
		]

	}
	]
}
