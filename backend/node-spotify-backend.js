var server = require('http').createServer(serveStatic),
	fs = require('fs'),
	path = require('path'),
	io = require('socket.io').listen(server);

server.listen(25000);
var baseFileUrl = '../frontend/';

function serveStatic(req, res) {
	var filePath = baseFileUrl + req.url;
	
	var contentType = 'text/html';
	var extension = path.extname(filePath);
	switch(extension) {
		case '.js':
			contentType = 'text/javascript';
			break;
		case '.css':
			contentType = 'text/css';
			break;
	}

	fs.exists(filePath, function(exists) {
		if(exists) {
			fs.readFile(filePath, function(error, content) {
				if(error) {
					res.writeHead(500);
					res.end();
				} else {
					res.writeHead(200, {'Content-Type': contentType });
					res.end(content, 'utf-8');
				}
			});
		} else {
			res.writeHead(404);
			res.end();
		}
	});
}

io.sockets.on('connection', function(socket) {
	socket.emit('init', { success: true });
});
