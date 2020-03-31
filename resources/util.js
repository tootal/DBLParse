function $(s) {
    if(s[0] == '#') {
        return document.getElementById(s.substr(1));
    }
    if(s[0] == '<' && s[s.length - 1] == '>') {
        return document.createElement(s.slice(1, -1));
    }
    return document.querySelectorAll(s);
}

$.load = function(src) {
    let script = $('<script>');
    script.src = src;
    document.body.appendChild(script);
    return script;
};

if(location.href.startsWith('file:')) {
    local = true;
    !function loadLocalTest() {
        let s = location.href;
        let name = s.slice(s.lastIndexOf('/') + 1, s.lastIndexOf('.'));
        $.load(name + '.test.js');
    }();
}

!function translate() {
    var tr = function(str) {
        return tr[str];
    };
    $.load('strings_zh.js');
    for(let node of $('[tr]')) {
        console.log(node.innerText);
    }
}();
