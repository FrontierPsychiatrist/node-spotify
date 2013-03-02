require.config( {
    paths: {
        'backbone' : 'libs/backbone',
        'underscore' : 'libs/underscore',
        'jquery' : 'libs/jquery'
    },
    shim: {
        'backbone' : {
            deps: ['underscore', 'jquery'],
            exports: 'Backbone'
        },

        'underscore' : {
            exports: '_'
        },

        'jquery' : {
            exports: '$'
        }
    }
});

require([
    'app'
], function(App) {
        App.initialize();
    }
);