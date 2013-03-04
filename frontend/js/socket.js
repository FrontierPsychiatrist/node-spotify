define([
    'io',
    'jquery'
], function(io, $) {
    var socket = io.connect('http://localhost:25000');
    socket.on('init', function(data) {
        $('#content').append('socket.io correctly loaded<br/>');
    })
});