{
	"targets": [
	{
		"target_name": "spotify",
		"sources": ["src/spotify.cc", "src/appkey.c", "src/SpotifyService.cc", "src/logindata.c"],
		"link_settings" : {
			"libraries": ["-lspotify"]
		},
		"cflags": ["-g"]

	}
	]
}
