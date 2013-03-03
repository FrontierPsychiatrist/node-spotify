define([
    'backbone',
    'underscore',
    'jquery'
], function (Backbone, _, $) {
    var PlaylistMenuItemView = Backbone.View.extend({

        tagName: 'li',

        template: _.template( $('#playlist-menuEntry').html() ),

        render: function() {
            this.$el.html( this.template( this.model ));
            return this;
        }
    });

    return PlaylistMenuItemView;
});