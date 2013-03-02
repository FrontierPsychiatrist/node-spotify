define([
    'jquery',
    'router'
], function($, router) {

        var initialize = function() {
            $('#content').html('Require JS correctly loaded<br/>');
            router.initialize();
        };

        return {
            initialize: initialize
        };
    }
);