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
        $('#key').innerHTML += `<li class="breadcrumb-item active">${key}</li>`;
    }
    $('#name').innerText = json.name;
    $('#title').innerHTML = json.title;
    for (let link of['url', 'ee']) {
        if (link in json) {
            $('#link').innerHTML += `<a href="${json[link]}" class="btn btn-outline-primary">${tr(link)}</a>`;
        }
    }
};

var stylish = function(data) {
    let json = JSON.parse(data);
    stylish_special(json);

    for (let item of items) {
        if (item in json) {
            $('#list').innerHTML += `<li class="list-group-item"><span class="font-weight-bold mr-1">${tr(item)}: </span>${json[item]}</li>`;
        }
    }
};

if (location.href.startsWith('qrc:')) {
    stylish($('#src').innerText);
} else {
    load('detail.test.js', function() {
        stylish(test.data);
    });
}