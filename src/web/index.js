var t;
var costTiming;
var costMsec;

load('echarts.config.js');

let g_data = {
    type: 'author',
    word: '',
    type_list: [
        'author',
        'title',
        'coauthor',
        'cograph',
        'keywords'
    ],
    header: {
        'author': ['', 'Title', 'Author(s)', 'Year'],
        'title': ['', 'Title', 'Author(s)', 'Modified'],
        'coauthor': ['', 'Co-Author(s)'],
        'keywords': ['', 'Title', 'Author(s)', 'Modified']
    },
    table: []
};

var vm_inputs = new Vue({
    el: '#inputs',
    data: g_data,
    methods: {
        handleInput: function() {
            search(vm_inputs.type, vm_inputs.word);
        }
    },
    watch: {
        type: function(v) {
            $('#type').selectpicker('val', v);
        }
    }
});

var vm_alert = new Vue({
    el: '#alert',
    data: {
        message: 'This is test message.',
        seen: false,
        type: 'info'
    },
    methods: {
        tr: tr
    }
});

Vue.component('home-page', {
    props: ['author'],
    template: `
<div class="link-card mt-5 mx-auto text-decoration-none">
    <div class="d-flex justify-content-between align-items-center">
        <div class="float-left text-left">
            <div class="my-1" style="font-size: 22px;" tr>Home Page</div>
            <div id="homepage-meta" class="text-muted"> {{ author }} </div>
        </div>
        <img src="homepage.svg" alt="homepage" class="float-right" width="60" height="60">
    </div>
</div>
`
});

var vm_result = new Vue({
    el: '#result2',
    data: g_data,
    methods: {
        homepageClick: function() {
            console.log('homepage click');
        },
        tr: tr
    }
});

var alertMsg = function(type, msg) {
    vm_alert.type = type;
    vm_alert.message = msg;
    vm_alert.seen = true;
    var t = setTimeout(function() {
        vm_alert.seen = false;
    }, 4000);
}

var clearT = function() {
    clearTimeout(t);
};

var search = function(type, word) {
    if (type == 'title' && ['Home Page'].indexOf(word) != -1) {
        alertMsg('warning', 'You can not search this title.');
        clearT();
    } else {
        costMsec = 0;
        costTiming = setInterval(function() {
            costMsec += 100;
        }, 100);
        // console.log('search ', type, word);
        if (location.href.startsWith('qrc:')) {
            finder.find(type, word);
        } else {
            if (word == eval(`test.data.${type}`)) {
                handleSearch(eval(`test.data.${type}_ret`));
            } else {
                handleSearch(test.data.not_found_ret);
            }
        }
    }
};

// atob : base64 -> str
// btoa : str -> base64

var searchAuthor = function(authorEle) {
    scrollTo(0, 0);
    let author = atob(authorEle.dataset.author);
    g_type = 'author';
    g_word = author;
    search('author', author);
};

var formatTitle = function(title, index) {
    let href = 'detail.html';
    if (location.href.startsWith('qrc:')) {
        href = `dblp://get/${index}`;
    }
    return `<a href="${href}">${title}</a>`;
};

var formatAuthor = function(author) {
    return `<span class="search-author-other" onclick="searchAuthor(this)" data-author="${btoa(author)}">${author}</span>`;
}

var formatAuthors = function(record) {
    if (typeof record.authors == "undefined") return "";
    let ref = record.authors;
    if (ref == null || ref == "") return "";
    for (let j = 0; j < ref.length; ++j) {
        if (vm_inputs.type == 'title' || ref[j] != vm_inputs.word) {
            ref[j] = formatAuthor(ref[j]);
        } else {
            ref[j] = `<span class="font-weight-bold">${ref[j]}</span>`;
        }
    }
    return ref.join('; ');
}

var handleHomePage = function(index) {
    if (location.href.startsWith('qrc:')) {
        document.getElementById('homepage').href = `dblp://get/${index}`;
    } else {
        document.getElementById('homepage').href = 'detail.html';
    }
    document.getElementById('homepage-meta').innerText = vm_inputs.word;
    document.getElementById('homepage').style.display = "block";
}

var getNodes = function(parentNode, childNodes, nodes) {
    var pnode;
    for (var i = 0; i < nodes.length; i++) {
        if (parentNode === nodes[i].nodename) {
            pnode = nodes[i];
        }
    }
    for (i = 0; i < childNodes.length; i++) {
        var flag = false;
        var level = pnode.nodelevel;
        nodes.forEach(item => {
            if (item.nodename === childNodes[i]) {
                flag = true;
                level = item.nodelevel;
            }
        });
        if (flag) {
            nodes.push({
                nodename: childNodes[i],
                nodelevel: level,
                parentnode: parentNode,
            });
        } else {
            nodes.push({
                nodename: childNodes[i],
                nodelevel: pnode.nodelevel + 1,
                parentnode: parentNode,
            });
        }
    }
}

var setNodeData = function(nodes, listdata) {
    var size = 40;
    for (var i = 0; i < nodes.length; i++) {
        var flag = false;
        listdata.forEach(item => {
            if (item.name === nodes[i].nodename) {
                flag = true;
            }
        });
        if (!flag) {
            listdata.push({
                category: nodes[i].nodelevel,
                name: nodes[i].nodename,
                symbolSize: size - nodes[i].nodelevel * 10,
                draggable: "true",
                symbol: "circle"
            });
        }
    }
}

var setLinkData = function(childList, parentnode, links) {
    for (var i = 0; i < childList.length; i++) {
        links.push({
            "source": childList[i],
            "target": parentnode,
            lineStyle: {
                normal: {
                    color: 'source',
                }
            }
        });
    }
}
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

var handleSearch = function(data) {
    if (data == "not_ready") return;
    let json = JSON.parse(data);
    if (json.length == 0) {
        alertMsg('danger', `${tr(vm_inputs.type)}${tr(' not found!')}`);
        clearT();
        return;
    }
    let table = [];
    if (g_data.type == 'coauthor') {
        for (let i = 0; i < json.length; ++i) {
            table.push([i+1, formatAuthor(json[i])]);
        }
    } else if (g_data.type == 'title' || g_data.type == 'keywords') {
        for (let i = 0; i < json.length; ++i) {
            table.push([i+1, formatTitle(json[i].title, i), formatAuthors(json[i]), json[i].mdate]);
        }
    } else if (g_data.type == 'author') {
        let label = 1;
        for (let i = 0; i < json.length; ++i) {
            if (json[i].title == "Home Page") {
                // handleHomePage(i);
                continue;
            } else if (json[i].title == "") {
                continue;
            }
            table.push([label, formatTitle(json[i].title, i), formatAuthors(json[i]), json[i].year]);
            label = label + 1;
        }
    } else if (g_data.type == 'cograph') {

        document.getElementById('result').style.display = "none";
        document.getElementById('coGraph').style.display = "block";

        document.getElementById("save").onclick = function() {
            var myChart = echarts.getInstanceByDom(document.getElementById("graph"));
            var url = myChart.getDataURL();
            finder.image(url, json[0].parentNode + "の合作关系图");
        };

        var myChart = echarts.init(document.getElementById('graph'));

        var listdata = [];
        var linksdata = [];

        var nodes = [{
            "nodename": json[0].parentNode,
            "nodelevel": 0,
            "parentnode": 0
        }];

        for (var i = 0; i < json.length; i++) {
            getNodes(json[i].parentNode, json[i].childNodes, nodes);
            setLinkData(json[i].childNodes, json[i].parentNode, linksdata);
        }
        setNodeData(nodes, listdata);

        var levels = 0;
        var legend_data = [];
        var series_categories = [];
        var temp = ["一", "二", "三", "四", "五"];
        for (var i = 0; i < nodes.length; i++) {
            levels = Math.max(levels, nodes[i].nodelevel);
        }
        for (var i = 0; i <= levels; i++) {

            legend_data.push({
                name: i === 0 ? '父节点' : '层级' + temp[i],
                icon: 'rect'
            });

            series_categories.push({
                name: i === 0 ? '父节点' : '层级' + temp[i],
                symbol: 'rect'
            });

        }

        option.title.text = json[0].parentNode + "の合作关系图";
        option.legend.data = legend_data;
        option.series[0].data = listdata;
        option.series[0].links = linksdata;
        option.series[0].categories = series_categories;


        myChart.setOption(option);

        document.getElementById("graph").firstChild.style.margin = '0 auto';

        var data = myChart._model.option.series[0].data;
        myChart.on("click", function(chartParam) {
            if (chartParam.borderColor) {
                vm_inputs.word = data[chartParam.dataIndex].name;
                search(vm_inputs.type, vm_inputs.word);
            }
        });
    }

    g_data.table = table;
    clearInterval(costTiming);
    if (g_data.type != 'cograph') {
        let msg = tr('Find %1 results. ').arg(table.length);
        if (costMsec >= 100) {
            msg += tr('(Cost time: %1 s)').arg(costMsec / 1000);
        }
        alertMsg('success', msg);
    }
};

var resSaveMes = function(data) {
    if (data) {
        alert(tr("Save successfully, please go to the index file directory to view."));
    } else {
        alert(tr("Save failed!"));
    }
}

if (location.href.startsWith('qrc:')) {
    load('qwebchannel.js', function() {
        new QWebChannel(qt.webChannelTransport, function(channel) {
            finder = channel.objects.finder;
            finder.ready.connect(handleSearch);
            finder.saveImg.connect(resSaveMes);
            finder.languageChanged.connect(function(lang) {
                language = lang;
            });
        });
    });
} else {
    load('index.test.js', function() {
        // test.coauthor;
    });
}

document.getElementById('word').focus();

window.addEventListener('language_change', function() {
    $('#type').selectpicker('destroy');
    $('#type').selectpicker();
});