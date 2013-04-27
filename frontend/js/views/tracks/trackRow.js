define([
    'backbone',
    'underscore',
    'jquery'
], function (Backbone, _, $) {
    var TrackRowView = Backbone.View.extend({
        tagName: 'tr',

        events: {
            'click .track-play': 'play'
        },

        template: _.template( $('#track-row').html() ),

        render: function () {
            this.$el.html( this.template( this.model ));
            return this;
        },

        play: function() {
            $('#now-playing').html(this.model.name)
        }

    });

    return TrackRowView;
});