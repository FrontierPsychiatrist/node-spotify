/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

/**
 * Saves collections of objects that should be checked for their isLoaded flag in the metadata_updated
 * callback along with a callback to use on them.
 * {
 *   objects: [],
 *   callback: function(object)... 
 * }
 **/
var notLoadedObjects = [];

/**
 * This method will be attached to the metadata_updated callback. It will
 * iterate over all objects in the notLoadedObjects array and check of they have been loaded with this call.
 * If so the provided callback is called. All objects that are not loaded will be saved to notLoadedObjects again.
 **/
function metadataUpdated() {
    var length = notLoadedObjects.length;
    for(var i = 0; i < length; i++) {
        var toUpdate = notLoadedObjects.shift();
        var newQueueItem = { objects: [], callback: toUpdate.callback };
        toUpdate.objects.forEach(function(object) {
        if(object.isLoaded) {
            toUpdate.callback(object);
        } else {
            newQueueItem.objects.push(object);
        }
      });
      if(newQueueItem.objects.length > 0) {
        notLoadedObjects.push(newQueueItem);
      }
    }
}

/**
 * Creates a new entry in notLoadedObjects containing all objects in the parameter objects along with the callback.
 **/
function waitForLoaded(objects, callback) {
    var notLoaded = {
        objects: objects,
        callback: callback
    };
    if(notLoaded.objects.length > 0) {
        notLoadedObjects.push(notLoaded);
    }
}

module.exports = {
    waitForLoaded: waitForLoaded,
    metadataUpdated: metadataUpdated
};