var alert = function(type, msg) {
    $('#alert').className = `mt-5 alert alert-${type}`;
    $('#alert').innerHTML = msg;
    $('#alert').style.display = 'block';
}

var search = function(type, word) {
    clearBefore();
    if (type == 'title' && ['Home Page'].indexOf(word) != -1) {
        alert('warning', tr('You can not search this title.'));
    } else {
        // console.log('search ', type, word);
        if (location.href.startsWith('qrc:')) {
            finder.find(type, word);
        } else {
            if (word == eval(`test.data.${type}`)) {
                handleSearch(eval(`test.data.${type}_ret`));
            } else {
                handleSearch(test.data.not_found_ret);
            }
        }
    }
};

// atob : base64 -> str
// btoa : str -> base64

var searchAuthor = function(authorEle) {
    scrollTo(0, 0);
    $('#type').value = 'author';
    let author = atob(authorEle.dataset.author);
    $('#word').value = author;
    search('author', author);
};

var formatTitle = function(record) {
    let href = 'detail.html';
    if (location.href.startsWith('qrc:')) {
        href = `dblp://${record.mdate}/${record.key}`;
    }
    return `<a href="${href}">${record.title}</a>`;
};

var formatAuthor = function(author) {
    return `<span class="search-author-other" onclick="searchAuthor(this)" data-author="${btoa(author)}">${author}</span>`;
}

var formatAuthors = function(record) {
    let ref = record.authors;
    for (let j = 0; j < ref.length; ++j) {
        if ($('#type') == 'title' || ref[j] != $('#word').value) {
            ref[j] = formatAuthor(ref[j]);
        } else {
            ref[j] = `<span class="font-weight-bold">${ref[j]}</span>`;
        }
    }
    return ref.join('; ');
}

var clearBefore = function() {
    $('#thead').innerHTML = "";
    $('#tbody').innerHTML = "";
    $('#homepage').style.display = "none";
    $('#alert').style.display = "none";
}

var handleHomePage = function(record) {
    if (location.href.startsWith('qrc:')) {
        $('#homepage').href = `dblp://${record.mdate}/${record.key}`;
    } else {
        $('#homepage').href = 'detail.html';
    }
    $('#homepage-meta').innerText = $('#word').value;
    $('#homepage').style.display = "block";
}

var setHeader = function(list) {
    let s = '';
    for (i of list) {
        s += `<th>${tr(i)}</th>`;
    }
    $('#thead').innerHTML = `<tr>${s}</tr>`;
};

var rowHTML = function(list) {
    let s = '';
    for (i of list) {
        s += `<td>${i}</td>`;
    }
    return `<tr>${s}</tr>`;
};

var handleSearch = function(data) {
    if (data == "not_ready") return ;
    let json = JSON.parse(data);
    // console.log(json);
    if (json.length == 0){
        alert('danger', `${tr($('#type').value)}${tr(' not found!')}`);
        return ;
    }
    let tbodyHTML = '';
    if ($('#type').value == 'coauthor') {
        setHeader(['', 'Co-Author(s)']);
        for (let i = 0; i < json.length; ++i) {
            tbodyHTML += rowHTML([i+1, formatAuthor(json[i])]);
        }
    } else if ($('#type').value == 'title') {
        setHeader(['', 'Title', 'Author(s)', 'Modified']);
        json.sort(function(x, y) {
            return parseInt(x.mdate) - parseInt(y.mdate);
        });
        for (let i = 0; i < json.length; ++i) {
            tbodyHTML += rowHTML([i+1, formatTitle(json[i]), formatAuthors(json[i]), json[i].mdate]);
        }
    } else if ($('#type').value == 'author') {
        setHeader(['', 'Title', 'Author(s)', 'Year']);
        json.sort(function(x, y) {
            return parseInt(x.year) - parseInt(y.year);
        });
        let label = 1;
        for (let i = 0; i < json.length; ++i) {
            if (json[i].title == "Home Page") {
                handleHomePage(json[i]);
                continue;
            }
            // tbodyHTML += rowHTML([label, formatTitle(json[i]), formatAuthors(json[i]), json[i].year]);
            tbodyHTML += `<tr><td>${label}</td><td>${formatTitle(json[i])}</td> <td width="30%">${formatAuthors(json[i])}</td><td>${json[i].year}</td></tr>`;
            label = label + 1;
        }
    }
    $('#tbody').innerHTML = tbodyHTML;
};

if (location.href.startsWith('qrc:')) {
    new QWebChannel(qt.webChannelTransport, function(channel) {
        finder = channel.objects.finder;
        finder.ready.connect(handleSearch);
    });
} else {
    $.load('index.test.js', function() {
        // test.author;
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