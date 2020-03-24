'use strict';

new QWebChannel(qt.webChannelTransport, function(channel) {
    var finder = channel.objects.finder;
    document.getElementById('search').onclick = function() {
        var word = document.getElementById('word').value;
        var type = document.getElementById('type').value;
        finder.find(word, type);
    }
    finder.ready.connect(function(data) {
        // console.log(data);
        var result = document.getElementById('result');
        var json = JSON.parse(data);
        for(var i = 0; i < json.length; ++i){
            var item = document.createElement('li');
            item.innerHTML = json[i].title;
            result.appendChild(item);
        }
    })
})