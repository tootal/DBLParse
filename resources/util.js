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
    let res = s;
    if(typeof strings == "object" && s in strings) {
        res = strings[s];
    }
    return `<span tr="${s}">${res}</span>`;
}

Object.defineProperty(window, 'language', {
    get: () => this.m_language,
    set: function(v) {
        if(v.indexOf('-') != -1) {
            v = v.split('-')[0];
        }
        this.m_language = v;
        $.load(`strings_${v}.js`).onload = function() {
            for(let node of $('[tr]')) {
                let src = node.getAttribute('tr');
                if(src == ""){
                    src = node.innerText;
                    node.setAttribute('tr', src);
                }
                if(src in strings) {
                    node.innerText = strings[src];
                } else {
                    node.innerText = src;
                }
            }
        };
    }
});
language = navigator.language;
