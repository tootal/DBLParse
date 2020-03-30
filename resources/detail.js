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
    var testdata = "{\n    \"authors\": [\n        \"Katja Lenz\",\n        \"Ingo Wegener\"\n    ],\n    \"ee\": \"https://doi.org/10.1016/0304-3975(91)90194-7\",\n    \"journal\": \"Theor. Comput. Sci.\",\n    \"key\": \"journals/tcs/LenzW91\",\n    \"mdate\": \"2017-05-28\",\n    \"name\": \"article\",\n    \"number\": \"2\",\n    \"pages\": \"257-268\",\n    \"title\": \"The Conjunctive Complexity of Quadratic Boolean Functions.\",\n    \"url\": \"https://dblp.uni-trier.de/db/journals/tcs/tcs81.html#LenzW91\",\n    \"volume\": \"81\",\n    \"year\": \"1991\"\n}\n";
}

var formatValue = function(str) {
    let e = new RegExp('(https?|ftp)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]');
    if(e.test(str)){
        return '<a href="' + str + '">' + str + '</a>';
    }else{
        return str;
    }
};

var stylish = function(data) {
    let json = JSON.parse(data);
    // console.log(json);
    $('#title').innerHTML = json.title;
    $('#name').innerHTML = json.name;
    for(let key in json){
        if(key == 'name') continue;
        $('#list').innerHTML += '<li><span class="key">' + key + ': </span>' + formatValue(json[key]) + '</li>';
    }
};

if(local) stylish(testdata);
else stylish($('#src').innerText);