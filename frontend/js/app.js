define([
    'jquery',
    'router',
    'views/login',
    'socket',
    'events',
    'bootstrap',
    'views/playlist/menu'
], function($, router, LoginView, socket, events, bootstrap, PlaylistMenuView) {

        //TODO: refactor this.
        var initialize = function() {
            $('#logoutButton').on('click', function() {
                socket.emit(events.logout);
            })
            router.initialize();

            socket.on(events.logged_in, function(data) {
                if(data.loggedIn) {
                    //request initial data
                    socket.emit(events.initial_data);
                } else {
                    var loginView = new LoginView();
                    $('#modal').html( loginView.render().el ).modal({keyboard: false});
                }
            });

            socket.on(events.initial_data, function (data) {
                $('#modal').modal('hide');
                for(var i = 0; i < data.length; i++) {
                    var playlistMenuView = new PlaylistMenuView({ model: data[i] });
                    $('#playlists').append(playlistMenuView.render().el);
                }
            });

            //Asked the backend, if it is logged in
            socket.emit(events.logged_in);
        };

        return {
            initialize: initialize
        };
    }
);