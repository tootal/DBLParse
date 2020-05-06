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
    //    console.log(json);

    let tbodyHTML = '';
    setHeader(['', 'Author(s)', 'ArticleNum']);

    for (let i = 0; i < json.length; ++i) {
        tbodyHTML += rowHTML([i + 1, json[i].author, json[i].articleNum]);
    }
    document.getElementById('tbody').innerHTML = tbodyHTML;
};

if (location.href.startsWith('qrc:')) {
    showData(document.getElementById('src').innerText);
} else {
    load('authorStac.test.js', function() {
        showData(test.data);
    });
}