function $(id) {
    if(typeof id == 'string'){
        if(id[0] == '#') {
            return document.getElementById(id.substr(1));
        }
        if(id[0] == '<' && id[id.length - 1] == '>') {
            return document.createElement(id.slice(1, -1));
        }
    }
}

var disable_title_word = [
    'Home Page'
];

var ele_word = $('#word');
var ele_type = $('#type');
var ele_result = $('#result');
var ele_search = $('#search');

var search = function() {
    if(ele_type.value == 'title' && 
        disable_title_word.indexOf(word.value) != -1){
            ele_result.innerHTML = 'NOT FOUND!';
        }
    else {
        finder.find(ele_word.value, ele_type.value);
    }
}

var searchAuthor = function(authorEle){
    ele_type.value = 'author';
    ele_word.value = authorEle.innerHTML;
    search();
}

var handleSearch = function(data) {
    var json = JSON.parse(data);
    if(json.length == 0) {
        ele_result.innerHTML = 'NOT FOUND!';
        return ;
    }

    if(ele_type.value == 'coauthor') {
        ele_result.innerHTML = '<tr> <th>Co-Author(s)</th> </tr>';
        for(var i = 0; i < json.length; ++i){
            ele_result.innerHTML += '<td>' + json[i] + '</td>';
        }
        return ;
    }
    ele_result.innerHTML = '<tr> <th>Title</th> <th>Author(s)</th> <th>Year</th> </tr>';

    for(var i = 0; i < json.length; ++i){
        var tr = $('<tr>');
        tr.innerHTML += '<td> <a href="dblp://key/'+ json[i].key +'">' + json[i].title + '</a> </td>';
        if(ele_type.value == 'author'){
            let ref = json[i].authors;
            for(var j = 0; j < ref.length; ++j){
                if(ref[j] == ele_word.value){
                    ref[j] = '<span class="search-author">' + ref[j] + '</span>';
                }else{
                    ref[j] = '<span class="search-author-other" onclick="searchAuthor(this)">' + ref[j] + '</span>';
                }
            }
        }
        tr.innerHTML += '<td>' + json[i].authors.join('; ') + '</td>';
        tr.innerHTML += '<td>' + json[i].year + '</td>';
        ele_result.appendChild(tr);
    }
}

new QWebChannel(qt.webChannelTransport, function(channel) {
    finder = channel.objects.finder;
    finder.ready.connect(handleSearch);
});

ele_search.onclick = search;
ele_word.addEventListener('keydown', function(e) {
    if(e.keyCode == 13) search();
});
ele_word.focus();