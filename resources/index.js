function $(id) {
    if(typeof id == 'string') {
        if(id[0] == '#') {
            return document.getElementById(id.substr(1));
        }
        if(id[0] == '<' && id[id.length - 1] == '>') {
            return document.createElement(id.slice(1, -1));
        }
    }
}

if(location.href.startsWith('file:')) {
    var local = true;
    var testdata_searchAuthor = "[\n    {\n        \"authors\": [\n            \"Katja Lenz\"\n        ],\n        \"key\": \"homepages/39/2320\",\n        \"mdate\": \"2017-01-10\",\n        \"title\": \"Home Page\",\n        \"year\": \"\"\n    },\n    {\n        \"authors\": [\n            \"Simone Fries\",\n            \"Katja Lenz\"\n        ],\n        \"key\": \"conf/informatiktage/FriesL09\",\n        \"mdate\": \"2009-04-28\",\n        \"title\": \"Anwendungen in virtueller Realit&auml;t.\",\n        \"year\": \"2009\"\n    },\n    {\n        \"authors\": [\n            \"Carsten Damm\",\n            \"Katja Lenz\"\n        ],\n        \"key\": \"tr/trier/MI93-05\",\n        \"mdate\": \"2017-06-08\",\n        \"title\": \"Symmetric Functions in AC<sup>0</sup>&#091;2&#093;\",\n        \"year\": \"1993\"\n    },\n    {\n        \"authors\": [\n            \"Katja Lenz\",\n            \"Ingo Wegener\"\n        ],\n        \"key\": \"journals/tcs/LenzW91\",\n        \"mdate\": \"2017-05-28\",\n        \"title\": \"The Conjunctive Complexity of Quadratic Boolean Functions.\",\n        \"year\": \"1991\"\n    },\n    {\n        \"authors\": [\n            \"Katja Lenz\",\n            \"Ingo Wegener\"\n        ],\n        \"key\": \"conf/csl/LenzW87\",\n        \"mdate\": \"2017-05-19\",\n        \"title\": \"The Conjunctive Complexity of Quadratic Boolean Functions.\",\n        \"year\": \"1987\"\n    },\n    {\n        \"authors\": [\n            \"Katja Lenz\"\n        ],\n        \"key\": \"phd/dnb/Lenz92\",\n        \"mdate\": \"2017-01-10\",\n        \"title\": \"Die Komplexit&auml;t Boolescher Funktionen in Schaltkreisen &uuml;ber der Basis _424&#8853;,&#923;_425.\",\n        \"year\": \"1992\"\n    }\n]\n";
}

var disable_title_word = [
    'Home Page'
];

var ele_word = $('#word');
var ele_type = $('#type');
var ele_info = $('#info');
var ele_thead = $('#thead');
var ele_tbody = $('#tbody');
var ele_search = $('#search');
var ele_homepage = $('#homepage');

var search = function(type, word) {
    if(type == 'title' && 
        disable_title_word.indexOf(word) != -1) {
            ele_result.innerHTML = 'NOT FOUND!';
        }
    else {
        finder.find(type, word);
    }
};

// atob : base64 -> str
// btoa : str -> base64

var searchAuthor = function(authorEle){
    scrollTo(0, 0);
    ele_type.value = 'author';
    let author = atob(authorEle.dataset.author);
    ele_word.value = author;
    search('author', author);
};

var formatTitle = function(record) {
    return '<a href="dblp://key/'+ record.key +'">' + record.title + '</a>';
};

var formatAuthors = function(record) {
    let ref = record.authors;
    for(let j = 0; j < ref.length; ++j) {
        if(ele_type == 'title' || ref[j] != ele_word.value){
            ref[j] = '<span class="search-author-other" onclick="searchAuthor(this)" data-author="'+ btoa(ref[j]) +'">' + ref[j] + '</span>';
        }else{
            ref[j] = '<span class="search-author">' + ref[j] + '</span>';
        }
    }
    return ref.join('; ');
}

var clearBefore = function() {
    ele_info.innerHTML = "";
    ele_tbody.innerHTML = "";
    ele_homepage.style.display = "none";
}

var handleHomePage = function(record) {
    $('#homepage').href = 'dblp://key/' + record.key;
    let metaText = 'Katja Lenz';
    if(!local) metaText = ele_word.value;
    $('#homepage-meta').innerText = metaText;
    ele_homepage.style.display = "block";
}



var handleSearch = function(data) {
    clearBefore();

    if(data == "noparsed") return ;
    let json = JSON.parse(data);
    // console.log(json);
    if(json.length == 0){
        ele_info.innerHTML = 'NOT FOUND';
        return ;
    }

    if(ele_type.value == 'coauthor') {
        ele_thead.innerHTML = '<tr> <th>Co-Author(s)</th> </tr>';
        for(let i = 0; i < json.length; ++i){
            ele_tbody.innerHTML += '<td class="search-author-other" onclick="searchAuthor(this)">' + json[i] + '</td>';
        }
    }else if(ele_type.value == 'title') {
        ele_thead.innerHTML = '<tr> <th> </th> <th>Title</th> <th>Author(s)</th> <th>Modified</th> </tr>';
        json.sort(function(x, y) {
            return parseInt(x.mdate) - parseInt(y.mdate);
        });
        for(let i = 0; i < json.length; ++i) {
            let tr = $('<tr>');
            tr.innerHTML += '<td>' + (i+1) + '</td>';
            tr.innerHTML += '<td>' + formatTitle(json[i]) + '</td>';
            tr.innerHTML += '<td>' + formatAuthors(json[i]) + '</td>';
            tr.innerHTML += '<td>' + json[i].mdate + '</td>';
            ele_tbody.appendChild(tr);
        }
    }else if(ele_type.value == 'author') {
        ele_thead.innerHTML = '<tr> <th> </th> <th>Title</th> <th>Author(s)</th> <th>Year</th> </tr>';
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
            ele_tbody.appendChild(tr);
            label = label + 1;
        }
    }
};

if(!local){
    new QWebChannel(qt.webChannelTransport, function(channel) {
        finder = channel.objects.finder;
        finder.ready.connect(handleSearch);
    });
}

var handleInput = function() {
    search(ele_type.value, ele_word.value);
}

ele_search.onclick = handleInput;
ele_word.addEventListener('keydown', function(e) {
    if(e.keyCode == 13) handleInput();
});
ele_word.focus();

if(local) handleSearch(testdata_searchAuthor);