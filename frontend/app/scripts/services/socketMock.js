'use strict';
/* jshint camelcase: false */
angular.module('node-spotify')
  .factory('socket', function(events) {
    var eventFunctions = {};
    var playlistNames = ['best of', 'Classical', 'Rock'];
    return {

        on: function(event, fun) {
            eventFunctions[event] = fun;
          },

        emit: function(event, inData) {
            var data = {};
            if(event === events.logged_in) {
              data.loggedIn = true;
            } else if(event === events.initial_data) {
              data = [
                {id: 0, name: 'best of'},
                {id: 1, name: 'Classical'},
                {id: 2, name: 'Rock'}
              ];
              for (var i = 3; i < 40; i++) {
                data.push( {id: i, name: 'Playlist ' + i});
              }
            } else if(event === events.playlist_tracks) {
              data.name = playlistNames[inData.id];
              var beirut = {'name': 'Beirut'};
              var qotsa = {'name': 'Queens of the Stone Age'};
              if(inData.id === 0) {
                var tracks = [
                    {'artists': [beirut], name: 'Mount Wroclai (idle days)'},
                    {'artists': [beirut], name: 'Gulag Orkestar'},
                    {'artists': [qotsa], name: 'First It Giveth'}
                  ];
                for(var j = 3; j<100; j++) {
                  tracks.push( {'artists': [qotsa], 'name': 'Qotsa Track ' + j});
                }
                data.tracks = tracks;
              } else if (inData.id === 1) {
                data.tracks = [
                    {'artists': [{'name': 'Wolfgang Amadeus Mozart'}], name: 'Klarinettenkonzert'},
                    {'artists': [{'name': 'Ludwig van Beethoven'}], name: 'Sinfonie Nr. 6'}
                  ];
              } else {
                data.tracks = [
                  {'artists': [qotsa], name: 'First It Giveth'},
                  {'artists': [{'name': 'Tyler'}], name: 'Yonkers'},
                  {'artists': [{'name': 'Katsumi'}, {'name': 'Ishinawa'}], name: 'Midori'}
                ];
              }
            }

            eventFunctions[event](data);
          }
      };
  });
