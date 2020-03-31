function $(s) {
    if(s[0] == '#') {
        return document.getElementById(s.substr(1));
    }
    if(s[0] == '<' && s[s.length - 1] == '>') {
        return document.createElement(s.slice(1, -1));
    }
    return document.querySelectorAll(s);
}

$.load = function(src, callback) {
    let script = $('<script>');
    script.src = src;
    document.body.appendChild(script);
    if(typeof callback == 'function')
        script.onload = callback;
    return script;
};

var tr = function(s) {
    if(typeof strings == "undefined") return s;
    if(s in strings) {
        return strings[s];
    } else {
        console.log(s);
        return s;
    }
}

Object.defineProperty(window, 'language', {
    get: () => this.m_language,
    set: function(v) {
        this.m_language = v;
        if(v == 'en-US') return ;
        $.load(`strings_${v}.js`).onload = function() {
            for(let node of $('[tr]')) {
                node.innerText = tr(node.innerText);
            }
        };
    }
});
language = navigator.language;
