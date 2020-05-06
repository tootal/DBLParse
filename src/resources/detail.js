let items = [
    'authors',
    'mdate',
    'journal',
    'number',
    'volume',
    'pages',
    'year',
    'month',
    'booktitle',
    'series',
    'note',
    'address',
    'cdrom',
    'publisher',
    'crossref',
    'isbn',
    'school',
    'chapter'
];

var stylish_special = function(json) {
    for (let key of json.key.split('/')) {
        document.getElementById('key').innerHTML += `<li class="breadcrumb-item active">${key}</li>`;
    }
    document.getElementById('name').innerText = json.name;
    document.getElementById('title').innerHTML = json.title;
    for (let link of ['url', 'ee']) {
        if (link in json) {
            document.getElementById('link').innerHTML += `<a href="${json[link]}" class="btn btn-outline-primary">${tr(link)}</a>`;
        }
    }
};

var stylish = function(data) {
    let json = JSON.parse(data);
    stylish_special(json);

    for (let item of items) {
        if (item in json) {
            document.getElementById('list').innerHTML += `<li class="list-group-item"><span class="font-weight-bold mr-1">${tr(item)}: </span>${json[item]}</li>`;
        }
    }
};

if (location.href.startsWith('qrc:')) {
    stylish(document.getElementById('src').innerText);
} else {
    load('detail.test.js', function() {
        stylish(test.data);
    });
}