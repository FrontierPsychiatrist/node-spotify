node-spotify
============
This is a project with the goal to run spotify on a Raspberry Pi and control it over a web interface.

nodeJS was chosen as the backend, so it includes a native nodeJS module to interact with spotify.

The backend is obviously a nodeJS app.

The frontend is a backbone.js app that communicates with the backend via websockets (sockets.io).

A webpage containing a detailed description is in the planning.

Right now the only thing possible is to login and a list of the playlists for the user should be sent to the browser and be
rendered.

Running
-------
First, you need a spotify premium account to use node-spotify. Facebook login is not possible, if you login to spotify via Facebook you need to create a device login.

Compiling
---------
First, you need an appkey from spotify. You can obtain one here: https://developer.spotify.com/technologies/libspotify/#application-keys.
Download the c implementation of the appkey and paste it into the variables in src/example_appkey.c. Rename it to src/appkey.c

Second, you need node-gyp installed. Install it via npm install -g node-gyp.

Third, you need libspotify installed to /usr/local/lib and /usr/local/include. OSX users can install it with brew install libspotify. If you use OSX and have
installed libspotify as a framework you need to edit the binding.gyp file. Remove "-lspotify" and replace it with "-framework", "libspotify" (with the comma).

If your on Linux, src/SpotifyService/SpotifyService.cc needs to be changed slightly. Replace

    #include <mach/clock.h>
    #include <mach/mach.h>

with

    #include <sys/time.h>

and

    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec + nextTimeout / 1000;
    ts.tv_nsec = mts.tv_nsec + (nextTimeout % 1000) * 1000;

with

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec = ts.tv_sec + nextTimeout / 1000;
    ts.tv_nsec = ts.tv_nsec + (nextTimeout % 1000) * 1000;

Yes, i am actually to lazy to write a preprocessor statement for this right now.

Almost done. Change into the main folder (where binding.gyp lies) and run node-gyp configure && node-gyp build --debug (debug switch is needed right now).

Hope it builds.

Then switch to the backend folder. If this is the first time, run npm install socket.io . Then run node node-spotify-backend.js . Point your browser to http://localhost:25000.
Login with your password, the playlists should load. If the playlists do not show up completly, just reload. Pressing logout will log you out but will not be visible in the frontend.
