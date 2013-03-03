define([
    'jquery',
    'router',
    'views/playlist/menu'
], function($, router, PlaylistMenuView) {

        var initialize = function() {
            $('#content').html('Require JS correctly loaded<br/>');
            router.initialize();

            var playlists  = [
                {'name': 'best of', 'id': 0},
                {'name': 'Beirut', 'id': 1},
                {'name': 'GY!BE', 'id': 2},
                {'name': 'Starred', 'id': 3},
                {'name': 'JME', 'id': 4},
                {'name': 'Jazz', 'id': 5}
            ];
            for (var i in playlists) {
                var playlistMenuView = new PlaylistMenuView({ model: playlists[i] });
                $('#playlists').append(playlistMenuView.render().el);
            }

        };

        return {
            initialize: initialize
        };
    }
);