node-spotify
============
Overview
--------
node-spotify is a node.js module to use the Spotify API in an easy manner.

The goal is to run spotify on a Raspberry Pi and control it over a web interface. So apart from the node.js module this also includes a node.js
app and a web frontend.

The backend is obviously a node.js app.

The nodeJS module is a native module written mainly in C++, the frontend is an angular.js app. Communication is done via socket.io.

A webpage for the project is here: http://www.node-spotify.com.

Requirements
------------
You need a spotify premium account to use node-spotify. This is a requirement for using libspotify, sorry.

Dependencies
------------
The following tools are required for building: node-gyp, bower, grunt-cli

Yout compiler must be able to translate C++11 lambdas (G++ 4.7 or Clang 3.2 should do).

The following libraries are required for building: libspotify.

Compiling
---------
Compiling was tested on Raspbian and OSX.

To compile from source you need an appkey from spotify. You can obtain one here: https://developer.spotify.com/technologies/libspotify/#application-keys.
Download the C implementation of the appkey and paste it into the variables in src/example_appkey.c. Rename it to src/appkey.c

If you use OSX and have installed libspotify as a framework you need to edit the binding.gyp file. Remove "-lspotify" and write instead as one link option:
"-framework OpenAL -framework libspotify". This is due to a bug in node-gyp that will eliminate a duplicate "-framework" entry from the link settings.

Almost done. Change into the main folder (where binding.gyp lies) and run node-gyp configure && node-gyp build --debug (debug switch is needed right now as
the include in the nodeJS app is hardcoded to the debug folder).

In the frontend folder, run bower install to install the required dependencies.

Then switch to the backend folder. If this is the first time, run npm install. Then run ./start.sh . Point your browser to http://localhost:25000.
Login with your password and your playlists should load.

Binary distribution
-------------------
As soon as it is possible there will be a binary distribution including everything you need compiled for the Raspberry Pi.
