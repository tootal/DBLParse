'use strict';

var disable_word = [
    'Home Page'
];

new QWebChannel(qt.webChannelTransport, function(channel) {
    var finder = channel.objects.finder;

    function search() {
        var word = document.getElementById('word').value;
        var type = document.getElementById('type').value;
        if(disable_word.indexOf(word) == -1) finder.find(word, type);
        else document.getElementById('result').innerHTML = 'NOT FOUND!';
    }
    document.getElementById('search').onclick = function(){ search() };
    document.getElementById('word').addEventListener('keydown', function(e) {
        if(e.keyCode == 13) search();
    });
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



window.onload = function() {
    if(document.readyState == 'complete') {
        document.getElementById('word').focus();
    }
}