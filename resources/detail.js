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
        $('#list').innerHTML += '<li><span class="key">' + tr(key) + ': </span>' + formatValue(json[key]) + '</li>';
    }
};

if(location.href.startsWith('file:')) {
    $.load('detail.test.js', function() {
        stylish(test.data);
    });
} else {
    stylish($('#src').innerText);
}
