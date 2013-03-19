node-spotify
============
This is a project with the goal to run spotify on a Raspberry Pi and control it over a web interface.

nodeJS was chosen as the backend, so it includes a native nodeJS module to interact with spotify.

The backend is obviously a nodeJS app.

The frontend is a backbone.js app that communicates with the backend via websockets (sockets.io).

A webpage containing a detailed description is in the planning.

Right now the only thing possible is to login and a list of the playlists for the user should be sent to the browser and be
rendered.

Compiling
---------

To compile from source an appkey from spotify is needed, unfortunately i cannot provide mine.