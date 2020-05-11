String.prototype.arg = function() {
    let args = [].slice.call(arguments, 0);
    if (args.length == 1 && typeof args[0] == "object") {
        return this.replace(/\{(\w+)\}/g, function(match, cap) {
            return args[0][cap] || match;
        });
    }
    return this.replace(/\%(\d+)/g, function(match, cap) {
        return String(args[cap - 1]) || match;
    });
}

load = function(src, callback) {
    let script = document.createElement('script');
    script.src = src;
    document.body.appendChild(script);
    if (typeof callback == 'function')
        script.onload = callback;
};

var tr = function(s) {
    if (s.startsWith('<span tr="')) return s;
    let res = s;
    if (typeof strings == "object" && s in strings) {
        res = strings[s];
    }
    return `<span tr="${s}">${res}</span>`;
}

Object.defineProperty(window, 'language', {
    get: () => this._language,
    set: function(v) {
        if (v.indexOf('-') != -1) {
            v = v.split('-')[0];
        }
        if (v.indexOf('_') != -1) {
            v = v.split('_')[0];
        }
        this._language = v;
        load(`strings_${v}.js`, function() {
            for (let node of document.querySelectorAll('[tr]')) {
                let src = node.getAttribute('tr');
                if (src == "") {
                    src = node.innerText;
                    node.setAttribute('tr', src);
                }
                if (src in strings) {
                    node.innerText = strings[src];
                } else {
                    node.innerText = src;
                }
            }
            window.dispatchEvent(new CustomEvent('language_change'));
        });
    }
});

window.onload = function() {
    language = navigator.language;
}