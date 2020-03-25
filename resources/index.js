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
        var json = JSON.parse(data);
        var table = document.getElementById('result');
        if(json.length == 0) {
            table.innerHTML = 'NOT FOUND!';
            return ;
        }
        table.innerHTML = '<tr> <th>Title</th> <th>Author(s)</th> <th>Year</th> </tr>';
        for(var i = 0; i < json.length; ++i){
            var tr = document.createElement('tr');
            tr.innerHTML += '<td>' + json[i].title + '</td>';
            tr.innerHTML += '<td>' + json[i].authors.join('; ') + '</td>';
            tr.innerHTML += '<td>' + json[i].year + '</td>';
            table.appendChild(tr);
        }
    })
})