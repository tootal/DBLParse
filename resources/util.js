function $(s) {
    if(s[0] == '#') {
        return document.getElementById(s.substr(1));
    }
    if(s[0] == '<' && s[s.length - 1] == '>') {
        return document.createElement(s.slice(1, -1));
    }
    return document.querySelectorAll(s);
}

!function loadLocalTest() {
    let s = location.href;
    if(s.startsWith('file:')) {
        let name = s.slice(s.lastIndexOf('/') + 1, s.lastIndexOf('.'));
        let script = $('<script>');
        script.src = name + '.test.js';
        document.body.appendChild(script);
    }
}();