node-spotify
============
node-spotify wraps the C library [libspotify](https://developer.spotify.com/technologies/libspotify) for usage with
NodeJS.

A webpage for the project is here: http://www.node-spotify.com.

I'm working toward version 1.0.0 which is not meant to provide everything libspotify offers. But I intend to wrap all
libspotify types and provide methods to access them where possible. Options and convenience features my lack, though.

Dependencies
------------
* libspotify must be installed
    - Linux users get it from [here](https://developer.spotify.com/technologies/libspotify/)
    - OSX users can use homebrew (`brew install libspotify`) or download it from
      [here](https://developer.spotify.com/technologies/libspotify/)
* an appkey for libspotify, get it [here](https://developer.spotify.com/technologies/libspotify/#application-keys).
  Choose binary, not C code.
* users of libspotify must have a Spotify premium account, this is required by Spotify.
* (Linux only) If you want audio the ALSA audio system, should be installed in most distributions [1]

[1] See building from source if you don't need/want platform-native audio (i.e. ALSA/OpenAL) in node-spotify.

Building from source
--------------------
node-spotify is prebuilt with [node-pre-gyp](https://www.npmjs.com/package/node-pre-gyp). If your NodeJS version or
architecture is not available it will build automatically from source.

If you want to force a build from source you can run `npm install node-spotify --build-from-source`. There's one special
option you can set when compiling node-spotify: `--native_audio=false` will compile/link no ALSA/OpenAL audio code
(depending on your platform). Default is `true`. This is both settable for node-gyp and npm.

node-spotify uses [Nan](https://github.com/nodejs/nan) to compile on different NodeJS versions. 0.10, 0.12, 4.x, 5.x, 6.x.
should all work.


If you use OSX and have installed libspotify as a framework you need to edit the binding.gyp file. Remove "-lspotify" and
write instead as one link option: `"-framework OpenAL -framework libspotify"`. This is due to a bug in node-gyp
that will eliminate a duplicate "-framework" entry from the link settings. If you have installed libspotify via homebrew
you don't need this step.

Used software
-------------
* Base64 encoder from https://github.com/superwills/NibbleAndAHalf
* Sound playback is heavily based on https://developer.spotify.com/docs/libspotify/12.1.51/examples.html
