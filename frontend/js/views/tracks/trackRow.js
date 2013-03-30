define([
    'backbone',
    'underscore',
    'jquery'
], function (Backbone, _, $) {
    var TrackRowView = Backbone.View.extend({
        tagName: 'tr',

        template: _.template( $('#track-row').html() ),

        render: function () {
            this.$el.html( this.template( this.model ));
            return this;
        }

    });

    return TrackRowView;
});