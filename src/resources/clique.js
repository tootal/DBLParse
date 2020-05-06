var setHeader = function(list) {
    let s = '';
    for (i of list) {
        s += `<th>${tr(i)}</th>`;
    }
    document.getElementById('thead').innerHTML = `<tr>${s}</tr>`;
};

var rowHTML = function(list) {
    let s = '';
    for (i of list) {
        s += `<td>${i}</td>`;
    }
    return `<tr>${s}</tr>`;
};

var showData = function(data) {
    let json = JSON.parse(data);

    let tbodyHTML = '';
    setHeader(['k', 'The number of k-cliques']);

    for (let i = 1;; ++i) {
        if (json.hasOwnProperty(i)) {
            tbodyHTML += rowHTML([i, json[i]]);
        } else {
            break;
        }
    }
    document.getElementById('tbody').innerHTML = tbodyHTML;
};

if (location.href.startsWith('qrc:')) {
    showData(document.getElementById('src').innerText);
} else {
    load('clique.test.js', function() {
        showData(test.data);
    });
}