var disable_word = [
    'Home Page'
];

new QWebChannel(qt.webChannelTransport, function(channel) {
    window.finder = channel.objects.finder;
});


function search() {
    var word = document.getElementById('word').value;
    var type = document.getElementById('type').value;
    if(disable_word.indexOf(word) == -1) finder.find(word, type);
    else document.getElementById('result').innerHTML = 'NOT FOUND!';
}

function searchAuthor(author){
    document.getElementById('type').value = 'author';
    document.getElementById('word').value = author.innerHTML;
    search();
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
    if(document.getElementById('type').value == 'coauthor') {
        table.innerHTML = '<tr> <th>Co-Author(s)</th> </tr>';
        for(var i = 0; i < json.length; ++i){
            table.innerHTML += '<td>' + json[i] + '</td>';
        }
        return ;
    }
    table.innerHTML = '<tr> <th>Title</th> <th>Author(s)</th> <th>Year</th> </tr>';

    for(var i = 0; i < json.length; ++i){
        var tr = document.createElement('tr');
        tr.innerHTML += '<td> <a href="dblp://key/'+ json[i].key +'">' + json[i].title + '</a> </td>';
        if(document.getElementById('type').value == 'author'){
            for(var j = 0; j < json[i].authors.length; ++j){
                if(json[i].authors[j] == document.getElementById('word').value){
                    json[i].authors[j] = '<span class="search-author">' + json[i].authors[j] + '</span>';
                }else{
                    json[i].authors[j] = '<span class="search-author-other" onclick="searchAuthor(this)">' + json[i].authors[j] + '</span>';
                }
            }
        }
        tr.innerHTML += '<td>' + json[i].authors.join('; ') + '</td>';
        tr.innerHTML += '<td>' + json[i].year + '</td>';
        table.appendChild(tr);
    }
});

document.getElementById('word').focus();