node-spotify
============
Overview
--------
node-spotify is a node.js module to use the Spotify API in an easy manner from node.js.

The node.js module is a native module written mainly in C++.

A webpage for the project is here: http://www.node-spotify.com.

Notes
-----
Currently I have decided to bring the pure node.js module to a somewhat complete state before I improve the sample backend and frontend. I'll try to keep the documentation up to date with the current implementation so you know how to use node-spotify.

Working towards version 1.0.0 node-spotify should include the following browsing features: the playlists of the logged in user, searching,
browsing albums and artists. Playback should be possible. Creation and editing of playlists should be possible. The usage of all
events on playlists should be possible.

Not planned for version 1.0.0 is the handling of other users playlists, the inbox system, scrobbling and managing other options like
cache folder etc.

Requirements
------------
You need a spotify premium account to build or use node-spotify. This is a requirement for using libspotify, sorry. You also need an appkey that comes with a premium account (https://developer.spotify.com/technologies/libspotify/#application-keys).

Dependencies
------------
node-gyp and libspotify are required for building. Under Linux you need libasound2-dev additionally. Install node-gyp via ```npm install -g node-gyp```.

Your compiler must be able to translate some C++11 features (std::shared_ptr, G++ 4.7 or Clang 3.2 should do).

Compiling
---------
Compiling was tested on Raspbian, Ubuntu and OSX. Due to the usage of pthreads I'm not sure if node-spotify will compile on Windows.


If you use OSX and have installed libspotify as a framework you need to edit the binding.gyp file. Remove "-lspotify" and write instead as one link option:
```"-framework OpenAL -framework libspotify"```. This is due to a bug in node-gyp that will eliminate a duplicate "-framework" entry from the link settings. If you
have installed libspotify via homebrew you don't need this step.

Change into the main folder (where binding.gyp lies) and run ```node-gyp configure && node-gyp build```.

Now the spotify module lies in ./build/Release/spotify.js. You can use it in a node.js program like so

```javascript
var spotify = require('./build/Release/spotify')( {
  appkeyFile: './spotify_appkey.key'
});
```

The appkey file can be obtained from https://developer.spotify.com/technologies/libspotify/#application-keys (choose binary, not C-code).

Binary distribution
-------------------
As of version 0.4.0 downloads of the pure compiled node.js module are available at http://www.node-spotify.com. I'll try to provide OSX, Linux x86_64 (ALSA) and Linux ARMv6hf (ALSA) builds.

Used software
-------------
* Base64 encoder from https://github.com/superwills/NibbleAndAHalf
