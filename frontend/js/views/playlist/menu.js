define([
    'backbone',
    'underscore',
    'jquery',
    'socket',
    'events'
], function (Backbone, _, $, socket, events) {
    var PlaylistMenuItemView = Backbone.View.extend({

        tagName: 'li',

        events: {
            'click a': 'getTracks'
        },

        template: _.template( $('#playlist-menuEntry').html() ),

        render: function () {
            this.$el.html( this.template( this.model ));
            return this;
        },

        getTracks: function() {
            socket.emit(events.playlist_tracks, {id: this.model.id});
        }
    });

    return PlaylistMenuItemView;
});