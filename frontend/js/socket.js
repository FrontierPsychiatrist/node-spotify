define([
    'io'
], function(io) {
    var socket = io.connect('http://localhost:25000');
    return socket;
});