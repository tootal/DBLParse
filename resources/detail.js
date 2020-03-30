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
    var testdata = "[\n    {\n        \"authors\": [\n            \"Carsten Damm\",\n            \"Katja Lenz\"\n        ],\n        \"journal\": \"Universit&auml;t Trier, Mathematik/Informatik, Forschungsbericht\",\n        \"key\": \"tr/trier/MI93-05\",\n        \"mdate\": \"2017-06-08\",\n        \"name\": \"article\",\n        \"title\": \"Symmetric Functions in AC<sup>0</sup>&#091;2&#093;\",\n        \"volume\": \"93-05\",\n        \"year\": \"1993\"\n    }\n]\n";
}

var stylish = function(data) {
    let json = JSON.parse(data)[0];
    // console.log(json);
    $('#title').innerHTML = json.title;
    $('#name').innerHTML = json.name;
    for(let key in json){
        if(key == 'name') continue;
        $('#list').innerHTML += '<li><span class="key">' + key + ': </span>' + json[key] + '</li>';
    }
};

if(local) stylish(testdata);
else stylish($('#src').innerText);