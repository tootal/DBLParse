var search = function(type, word) {
    if(type == 'title' && ['Home Page'].indexOf(word) != -1) {
        ele_result.innerHTML = tr('NOT FOUND!');
    } else {
        // console.log('search ', type, word);
        finder.find(type, word);
    }
};

// atob : base64 -> str
// btoa : str -> base64

var searchAuthor = function(authorEle){
    scrollTo(0, 0);
    $('#type').value = 'author';
    let author = atob(authorEle.dataset.author);
    $('#word').value = author;
    search('author', author);
};

var formatTitle = function(record) {
    return '<a href="dblp://'+ record.mdate + '/'+ record.key +'">' + record.title + '</a>';
};

var formatAuthor = function(author) {
    return '<span class="search-author-other" onclick="searchAuthor(this)" data-author="'+ btoa(author) +'">' + author + '</span>';
}

var formatAuthors = function(record) {
    let ref = record.authors;
    for(let j = 0; j < ref.length; ++j) {
        if($('#type') == 'title' || ref[j] != $('#word').value){
            ref[j] = formatAuthor(ref[j]);
        }else{
            ref[j] = '<span class="search-author">' + ref[j] + '</span>';
        }
    }
    return ref.join('; ');
}

var clearBefore = function() {
    $('#info').innerHTML = "";
    $('#thead').innerHTML = "";
    $('#tbody').innerHTML = "";
    $('#homepage').style.display = "none";
    $('#info').style.display = "none";
}

var handleHomePage = function(record) {
    $('#homepage').href = 'dblp://' + record.mdate + '/' + record.key;
    metaText = $('#word').value;
    $('#homepage-meta').innerText = metaText;
    $('#homepage').style.display = "block";
}



var handleSearch = function(data) {
    clearBefore();

    if(data == "not_ready") return ;
    let json = JSON.parse(data);
    // console.log(json);
    if(json.length == 0){
        $('#info').innerHTML = tr("NOT FOUND!");
        $('#info').style.display = "block";
        return ;
    }

    if($('#type').value == 'coauthor') {
        $('#thead').innerHTML = '<tr> <th> </th> <th>' + tr('Co-Author(s)') + '</th> </tr>';
        for(let i = 0; i < json.length; ++i){
            $('#tbody').innerHTML += '<tr><td>' + (i+1) + '</td><td>' + formatAuthor(json[i]) + '</td></tr>';
        }
    }else if($('#type').value == 'title') {
        $('#thead').innerHTML = '<tr> <th> </th> <th>' + tr('Title') + '</th> <th>' + tr('Author(s)') + '</th> <th>' + tr('Modified') + '</th> </tr>';
        json.sort(function(x, y) {
            return parseInt(x.mdate) - parseInt(y.mdate);
        });
        for(let i = 0; i < json.length; ++i) {
            let tr = $('<tr>');
            tr.innerHTML += '<td>' + (i+1) + '</td>';
            tr.innerHTML += '<td>' + formatTitle(json[i]) + '</td>';
            tr.innerHTML += '<td>' + formatAuthors(json[i]) + '</td>';
            tr.innerHTML += '<td>' + json[i].mdate + '</td>';
            $('#tbody').appendChild(tr);
        }
    }else if($('#type').value == 'author') {
        $('#thead').innerHTML = '<tr> <th> </th> <th>' + tr('Title') + '</th> <th>' + tr('Author(s)') + '</th> <th>' + tr('Year') + '</th> </tr>';
        json.sort(function(x, y) {
            return parseInt(x.year) - parseInt(y.year);
        });
        let label = 1;
        for(let i = 0; i < json.length; ++i){
            if(json[i].title == "Home Page"){
                handleHomePage(json[i]);
                continue;
            }
            let tr = $('<tr>');
            tr.innerHTML += '<td>' + label + '</td>';
            tr.innerHTML += '<td>' + formatTitle(json[i]) + '</td>';
            tr.innerHTML += '<td>' + formatAuthors(json[i]) + '</td>';
            tr.innerHTML += '<td>' + json[i].year + '</td>';
            $('#tbody').appendChild(tr);
            label = label + 1;
        }
    }
};

if(typeof QWebChannel != "undefined"){
    new QWebChannel(qt.webChannelTransport, function(channel) {
        finder = channel.objects.finder;
        finder.ready.connect(handleSearch);
    });
}

var handleInput = function() {
    search($('#type').value, $('#word').value);
}

$('#search').onclick = handleInput;
$('#word').addEventListener('keydown', function(e) {
    if(e.keyCode == 13) handleInput();
});

$('#word').focus();

if(location.href.startsWith('file:')) {
    $.load('index.test.js', function() {
        // test.coauthor;
    });
}