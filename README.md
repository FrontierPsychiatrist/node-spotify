node-spotify
============
Overview
--------
node-spotify is a nodeJS module to use the Spotify API in a easy manner.

The goal is to run spotify on a Raspberry Pi and control it over a web interface. So apart from the nodeJS module this also includes a nodeJS
app and a web frontend.

The backend is obviously a nodeJS app.

The nodeJS module is a native module written mainly in C++, the frontend is an angular.js app. Communication is done via socket.io.

A webpage for the project is here: http://www.node-spotify.com, put it is still being filled with content.

Requirements
------------
You need a spotify premium account to use node-spotify. This is a requiremnt for using libspotify, sorry.

Compiling
---------
Compiling was tested on Raspbian and OSX.

To compile from source you need an appkey from spotify. You can obtain one here: https://developer.spotify.com/technologies/libspotify/#application-keys.
Download the C implementation of the appkey and paste it into the variables in src/example_appkey.c. Rename it to src/appkey.c

You need node-gyp installed. Install it via npm install -g node-gyp.

libspotify needs to be on your linker/include path. OSX users can install it with brew install libspotify. If you use OSX and have
installed libspotify as a framework you need to edit the binding.gyp file. Remove "-lspotify" and replace it with "-framework", "libspotify" (with the comma).

Almost done. Change into the main folder (where binding.gyp lies) and run node-gyp configure && node-gyp build --debug (debug switch is needed right now as
the include in the nodeJS app is hardcoded to the debug folder).

Then switch to the backend folder. If this is the first time, run npm install socket.io . Then run node node-spotify-backend.js . Point your browser to http://localhost:25000/index.html .
Login with your password, the playlists should load. If the playlists do not show up completly, just reload. Pressing logout will log you out but will not be visible in the frontend.

Binary distribution
-------------------
As soon as it is possible there will be a binary distribution including everything you need compiled for the Raspberry Pi.
