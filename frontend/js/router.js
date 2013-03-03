define([
    'backbone'
], function(Backbone) {
    var AppRouter = Backbone.Router.extend({
        routes : {
            '' : 'home',
            'playlist/:id' : 'playlist'
        },

        home: function() {
            $('#content').append('Router correctly initialized<br/>');
        },

        playlist: function(id) {
            $('#content').append('Switched to playlist ' + id + '<br/>');
        }
    });

    var initialize = function() {
        var appRouter = new AppRouter();
        Backbone.history.start();
    };
    return { initialize: initialize };
});