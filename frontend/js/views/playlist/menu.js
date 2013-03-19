define([
    'backbone',
    'underscore',
    'jquery'
], function (Backbone, _, $) {
    var PlaylistMenuItemView = Backbone.View.extend({

        tagName: 'li',

        template: _.template( $('#playlist-menuEntry').html() ),

        render: function() {
            this.model.id = 0;//TODO: obviously garbage. Id is not implemented down to the native model yet.
            this.$el.html( this.template( this.model ));
            return this;
        }
    });

    return PlaylistMenuItemView;
});