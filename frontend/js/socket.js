define([
    'io'
], function(io) {
    var socket = io.connect('http://192.168.178.22:25000/');
    return socket;
});
