define([
    'backbone',
    'jquery',
    'underscore',
    'socket',
    'events'
], function (Backbone, $, _, socket, events) {

    var LoginView = Backbone.View.extend({
        template: _.template($('#modal-template').html()),

        events: {
            'click #modalConfirmButton': 'login'
        },

        render: function() {
            this.$el.html( this.template( { label: 'Login', closable: false, buttonText: 'Login', body: $('#loginformTemplate').text() }));
            return this;
        },

        login: function(event) {
            var formData = $('#loginForm').serializeArray();
            socket.emit(events.login, {username: formData[0].value, password: formData[1].value});
        }
    });

    return LoginView;
});

