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

var stylish = function(data) {
    let json = JSON.parse(data);
    $('#name').innerText = json.name;
    $('#title').innerHTML = json.title;
    
    for (let item of items) {
        if (item in json) {
            $('#list').innerHTML += `<li class="list-group-item"><span class="font-weight-bold mr-1">${tr(item)}: </span>${json[item]}</li>`;
        }
    }
};

if (location.href.startsWith('qrc:')) {
    stylish($('#src').innerText);
} else {
    $.load('detail.test.js', function() {
        stylish(test.data);
    });
}
