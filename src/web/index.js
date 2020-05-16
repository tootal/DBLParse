const LENGTH_LIMIT = 2000;
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
    header_width: {
        'author': ['5%', '50%', '35%', '10%'],
        'title': ['5%', '50%', '35%', '10%'],
        'coauthor': ['20%', '80%'],
        'keywords': ['5%', '50%', '35%', '10%']
    },
    table: [],
    status: 'init',
    homepage_id: 0
};

var vm_inputs = new Vue({
    el: '#inputs',
    data: g_data,
    methods: {
        handleInput: function() {
            this.status = 'init';
            search(vm_inputs.type, vm_inputs.word);
        }
    },
    watch: {
        type: function(v) {
            $('#type').selectpicker('val', v);
            this.status = 'init';
            document.getElementById('coGraph').style.display = "none";
        },
        word: function() {
            this.status = 'init';
            document.getElementById('coGraph').style.display = "none";
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

var vm_result = new Vue({
    el: '#result',
    data: g_data,
    methods: {
        homepageClick: function() {
            // console.log('homepage click');
        },
        tr: tr
    },
    computed: {
        homepageHref: function() {
            if (location.href.startsWith('qrc:')) {
                return `dblp://get/${this.homepage_id}`;
            } else {
                return 'detail.html';
            }
        }
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
    g_data.type = 'author';
    g_data.word = author;
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
    if (typeof record.author == "undefined") return "";
    let ref = record.author;
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

var handleSearch = function(data) {
    if (data == "not_ready") return;
    let json = JSON.parse(data);
    // console.log(json);
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
            table.push([i+1, formatTitle(json[i].title[0], i), formatAuthors(json[i]), json[i].mdate[0]]);
        }
    } else if (g_data.type == 'author') {
        let label = 1;
        vm_result.homepage_id = -1;
        for (let i = 0; i < json.length; ++i) {
            if (json[i].title == "Home Page") {
                vm_result.homepage_id = i;
                continue;
            } else if (json[i].title == "") {
                continue;
            }
            table.push([label, formatTitle(json[i].title[0], i), formatAuthors(json[i]), json[i].year[0]]);
            label = label + 1;
        }
    } else if (g_data.type == 'cograph') {

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
        let type = 'success';
        if (table.length == LENGTH_LIMIT) {
            msg = tr('Too many results, only the first %1 are displayed. ').arg(LENGTH_LIMIT);
            type = 'warning';
        }
        if (costMsec >= 100) {
            msg += tr('(Cost time: %1 s)').arg(costMsec / 1000);
        }
        alertMsg(type, msg);
        g_data.status = 'show_result';
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