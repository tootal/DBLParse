var setHeader = function (list) {
    let s = '';
    for (i of list) {
        s += `<th>${tr(i)}</th>`;
    }
    document.getElementById('thead').innerHTML = `<tr>${s}</tr>`;
};

var rowHTML = function (list) {
    let s = '';
    for (i of list) {
        s += `<td>${i}</td>`;
    }
    return `<tr>${s}</tr>`;
};

var showData = function (data) {
    let json = JSON.parse(data);
    console.log(json);

    let tbodyHTML = '';
    setHeader(['Year', 'keyWord(s)']);

    for (let i = 0; i < json.length; ++i) {
        let words = '';
        for (let j = 0; j < json[i].words.length; ++j) {
            words += json[i].words[j].word + '(' + json[i].words[j].count + ')  ';
        }

        tbodyHTML += rowHTML([json[i].year, json[i].keyWord]);
    }
    document.getElementById('tbody').innerHTML = tbodyHTML;
};

if (location.href.startsWith('qrc:')) {
    showData($('#src').innerText);
} else {
    $.load('authorStac.test.js', function () {
        //        console.log('test');
        showData(test.data);
    });
}
