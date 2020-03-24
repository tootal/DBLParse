'use strict';

new QWebChannel(qt.webChannelTransport, function(channel) {
    var finder = channel.objects.finder;
    document.getElementById('search').onclick = function() {
        var word = document.getElementById('word').value;
        var type = document.getElementById('type').value;
        finder.find(word, type);
    }
})