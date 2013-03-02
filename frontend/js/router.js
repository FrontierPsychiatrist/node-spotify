define([
    'backbone'
], function(Backbone) {
    var AppRouter = Backbone.Router.extend({
        routes : {
            '' : 'home'
        },

        home: function() {
            $('#content').append('Router correctly initialized<br/>');
        }
    });

    var initialize = function() {
        var appRouter = new AppRouter();
        Backbone.history.start();
    };
    return { initialize: initialize };
});