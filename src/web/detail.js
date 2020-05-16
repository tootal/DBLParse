special_name = [
    'key',
    'name',
    'title',
    'url',
    'ee'
];

var stylish_special = function(json) {
    for (let key of json.key[0].split('/')) {
        document.getElementById('key').innerHTML += `<li class="breadcrumb-item active">${key}</li>`;
    }
    document.getElementById('name').innerText = json.name[0];
    document.getElementById('title').innerHTML = json.title[0];
    for (let link of ['url', 'ee']) {
        if (link in json) {
            for (let i of json[link]) {
                let href = i;
                if (i.indexOf('://') == -1) href = 'https://dblp.org/' + href;
                document.getElementById('link').innerHTML += 
                `<a href="${href}" class="btn btn-outline-primary">${tr('ee')}</a>`;
            }
        }
    }
};

var stylish = function(data) {
    let json = JSON.parse(data);
    stylish_special(json);

    for (let i in json) {
        if (special_name.indexOf(i) != -1) continue;
        document.getElementById('list').innerHTML += 
            `<li class="list-group-item"><span class="font-weight-bold mr-1">${tr(i)}: </span>${json[i].join(', ')}</li>`;
    }
};

if (location.href.startsWith('qrc:')) {
    stylish(document.getElementById('src').innerText);
} else {
    load('detail.test.js', function() {
        stylish(test.data);
    });
}