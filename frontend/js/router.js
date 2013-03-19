define([
    'backbone'
], function(Backbone) {
    var AppRouter = Backbone.Router.extend({
        routes : {
            '' : 'home',
            'playlist/:id' : 'playlist'
        },

        home: function() {
        },

        playlist: function(id) {
        }
    });

    var initialize = function() {
        var appRouter = new AppRouter();
        Backbone.history.start();
    };
    return { initialize: initialize };
});