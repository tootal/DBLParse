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
    var testdata_author = 'Katja Lenz';
    var testdata_title = 'The Conjunctive Complexity of Quadratic Boolean Functions.';
    var testdata_searchAuthor = "[\n    {\n        \"authors\": [\n            \"Katja Lenz\"\n        ],\n        \"key\": \"homepages/39/2320\",\n        \"mdate\": \"2017-01-10\",\n        \"title\": \"Home Page\",\n        \"year\": \"\"\n    },\n    {\n        \"authors\": [\n            \"Simone Fries\",\n            \"Katja Lenz\"\n        ],\n        \"key\": \"conf/informatiktage/FriesL09\",\n        \"mdate\": \"2009-04-28\",\n        \"title\": \"Anwendungen in virtueller Realit&auml;t.\",\n        \"year\": \"2009\"\n    },\n    {\n        \"authors\": [\n            \"Carsten Damm\",\n            \"Katja Lenz\"\n        ],\n        \"key\": \"tr/trier/MI93-05\",\n        \"mdate\": \"2017-06-08\",\n        \"title\": \"Symmetric Functions in AC<sup>0</sup>&#091;2&#093;\",\n        \"year\": \"1993\"\n    },\n    {\n        \"authors\": [\n            \"Katja Lenz\",\n            \"Ingo Wegener\"\n        ],\n        \"key\": \"journals/tcs/LenzW91\",\n        \"mdate\": \"2017-05-28\",\n        \"title\": \"The Conjunctive Complexity of Quadratic Boolean Functions.\",\n        \"year\": \"1991\"\n    },\n    {\n        \"authors\": [\n            \"Katja Lenz\",\n            \"Ingo Wegener\"\n        ],\n        \"key\": \"conf/csl/LenzW87\",\n        \"mdate\": \"2017-05-19\",\n        \"title\": \"The Conjunctive Complexity of Quadratic Boolean Functions.\",\n        \"year\": \"1987\"\n    },\n    {\n        \"authors\": [\n            \"Katja Lenz\"\n        ],\n        \"key\": \"phd/dnb/Lenz92\",\n        \"mdate\": \"2017-01-10\",\n        \"title\": \"Die Komplexit&auml;t Boolescher Funktionen in Schaltkreisen &uuml;ber der Basis _424&#8853;,&#923;_425.\",\n        \"year\": \"1992\"\n    }\n]\n";
    var testdata_notFound = '[]';
    var testdata_searchCoauthor = "[\n    \"Ingo Wegener\",\n    \"Simone Fries\",\n    \"Carsten Damm\"\n]\n";
    var testdata_searchTitle = "[\n    {\n        \"authors\": [\n            \"Katja Lenz\",\n            \"Ingo Wegener\"\n        ],\n        \"ee\": \"https://doi.org/10.1016/0304-3975(91)90194-7\",\n        \"journal\": \"Theor. Comput. Sci.\",\n        \"key\": \"journals/tcs/LenzW91\",\n        \"mdate\": \"2017-05-28\",\n        \"name\": \"article\",\n        \"number\": \"2\",\n        \"pages\": \"257-268\",\n        \"title\": \"The Conjunctive Complexity of Quadratic Boolean Functions.\",\n        \"url\": \"https://dblp.uni-trier.de/db/journals/tcs/tcs81.html#LenzW91\",\n        \"volume\": \"81\",\n        \"year\": \"1991\"\n    },\n    {\n        \"authors\": [\n            \"Katja Lenz\",\n            \"Ingo Wegener\"\n        ],\n        \"booktitle\": \"CSL\",\n        \"crossref\": \"conf/csl/1987\",\n        \"ee\": \"https://doi.org/10.1007/3-540-50241-6_35\",\n        \"key\": \"conf/csl/LenzW87\",\n        \"mdate\": \"2017-05-19\",\n        \"name\": \"inproceedings\",\n        \"pages\": \"138-150\",\n        \"title\": \"The Conjunctive Complexity of Quadratic Boolean Functions.\",\n        \"url\": \"https://dblp.uni-trier.de/db/conf/csl/csl87.html#LenzW87\",\n        \"year\": \"1987\"\n    }\n]\n";
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
    return '<a href="dblp://'+ record.mdate + '/'+ record.key +'">' + record.title + '</a>';
};

var formatAuthor = function(author) {
    return '<span class="search-author-other" onclick="searchAuthor(this)" data-author="'+ btoa(author) +'">' + author + '</span>';
}

var formatAuthors = function(record) {
    let ref = record.authors;
    for(let j = 0; j < ref.length; ++j) {
        if(ele_type == 'title' || ref[j] != ele_word.value){
            ref[j] = formatAuthor(ref[j]);
        }else{
            ref[j] = '<span class="search-author">' + ref[j] + '</span>';
        }
    }
    return ref.join('; ');
}

var clearBefore = function() {
    ele_info.innerHTML = "";
    ele_thead.innerHTML = "";
    ele_tbody.innerHTML = "";
    ele_homepage.style.display = "none";
    ele_info.style.display = "none";
}

var handleHomePage = function(record) {
    $('#homepage').href = 'dblp://' + record.mdate + '/' + record.key;
    let metaText = 'Katja Lenz';
    if(!local) metaText = ele_word.value;
    $('#homepage-meta').innerText = metaText;
    ele_homepage.style.display = "block";
}



var handleSearch = function(data) {
    clearBefore();

    if(data == "not_ready") return ;
    let json = JSON.parse(data);
    // console.log(json);
    if(json.length == 0){
        ele_info.innerHTML = "NOT FOUND!";
        ele_info.style.display = "block";
        return ;
    }

    if(ele_type.value == 'coauthor') {
        ele_thead.innerHTML = '<tr> <th> </th> <th>Co-Author(s)</th> </tr>';
        for(let i = 0; i < json.length; ++i){
            ele_tbody.innerHTML += '<tr><td>' + (i+1) + '</td><td>' + formatAuthor(json[i]) + '</td></tr>';
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

if(local){
    ele_type.value = 'title';
    // ele_word.value = testdata_author;
    ele_word.value = testdata_title;
    // handleSearch(testdata_searchAuthor);
    // handleSearch(testdata_notFound);
    // handleSearch(testdata_searchCoauthor);
    handleSearch(testdata_searchTitle);
}

ele_word.focus();