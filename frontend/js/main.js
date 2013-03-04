require.config( {
    paths: {
        'backbone' : 'libs/backbone',
        'underscore' : 'libs/underscore',
        'jquery' : 'libs/jquery',
		'io': '/socket.io/socket.io.js'
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
        },

		'io' : {
			exports: 'io'
		}
    }
});

require([
    'app'
], function(App) {
        App.initialize();
    }
);
