node-spotify
============
This is a project with the goal to run spotify on a Raspberry Pi and control it over a web interface.

nodeJS was chosen as the backend, so it includes a native nodeJS module to interact with spotify.

The backend is obviously a nodeJS app.

The frontend is a backbone.js app that communicates with the backend via websockets (sockets.io).

A webpage for the project is here: http://www.node-spotify.com, put it is still being filled with content.

Right now the only thing possible is to login and a list of the playlists for the user should be sent to the browser and be
rendered. You can click on a playlist and the tracks should be loaded.

Running
-------
You need a spotify premium account to use node-spotify.

Compiling
---------
First, you need an appkey from spotify. You can obtain one here: https://developer.spotify.com/technologies/libspotify/#application-keys.
Download the c implementation of the appkey and paste it into the variables in src/example_appkey.c. Rename it to src/appkey.c

Second, you need node-gyp installed. Install it via npm install -g node-gyp.

Third, you need libspotify installed to /usr/local/lib and /usr/local/include. OSX users can install it with brew install libspotify. If you use OSX and have
installed libspotify as a framework you need to edit the binding.gyp file. Remove "-lspotify" and replace it with "-framework", "libspotify" (with the comma).

Almost done. Change into the main folder (where binding.gyp lies) and run node-gyp configure && node-gyp build --debug (debug switch is needed right now).

Hope it builds.

Then switch to the backend folder. If this is the first time, run npm install socket.io . Then run node node-spotify-backend.js . Point your browser to http://localhost:25000/index.html .
Login with your password, the playlists should load. If the playlists do not show up completly, just reload. Pressing logout will log you out but will not be visible in the frontend.
