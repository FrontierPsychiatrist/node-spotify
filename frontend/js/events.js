(function(define) {

	define([], function() {
	var events = {
		/* Playlists */
		playlist_new: 'playlist_new',
		playlist_renamed: 'playlist_renamed',
        playlist_tracks: 'playlist_tracks',

		/* general */
		initial_data: 'initial_data',
        logged_in: 'logged_in',
        login: 'login',
        logout: 'logout',
		message: 'message'
	};

	return events;
	});
	
})(typeof define === 'function' 
	? define 
	: function(mods, exports) { module.exports = exports(); });
