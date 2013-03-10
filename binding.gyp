{
	"targets": [
	{
		"target_name": "spotify",
		"sources": ["src/spotify.cc", "src/appkey.c", "src/SpotifyService/SpotifyService.cc",
			"src/spotify/V8Callable.cc", "src/spotify/SpotifyWrapped.cc",
			"src/spotify/Playlist.cc", "src/spotify/PlaylistContainer.cc",
			"src/SpotifyService/PlaylistCallbacks.cc",
			"src/SpotifyService/SessionCallbacks.cc"
		],
		"link_settings" : {
			"libraries": ["-lspotify"]
		},
		"cflags": ["-g"]

	}
	]
}
