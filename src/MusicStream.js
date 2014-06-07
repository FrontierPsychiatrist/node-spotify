var Readable = require('stream').Readable;
var util = require('util');

util.inherits(MusicStream, Readable);

function MusicStream() {
  Readable.call(this, {
    highWaterMark: 8192*21 //ca 1 second for 44.1kHz/2 channels
  });
  var self = this;
  this.needMoreData = false;
  this.musicDelivery = function(err, data) {
    self.needMoreData = self.push(data);
    return self.needMoreData;
  }
}

MusicStream.prototype._read = function(size) {
  if(!this.needMoreData) {
    //If we stopped pushing data before we now tell the session to continue delivering audio data
    this.needMoreDataSetter(true);
  }
}

module.exports = MusicStream;