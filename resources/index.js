var disable_title_word = [
    'Home Page'
];

var search = function(type, word) {
    if(type == 'title' && 
        disable_title_word.indexOf(word) != -1) {
            ele_result.innerHTML = tr('NOT FOUND!');
        }
    else {
        // console.log('search ', type, word);
        finder.find(type, word);
    }
};

// atob : base64 -> str
// btoa : str -> base64

var searchAuthor = function(authorEle){
    scrollTo(0, 0);
    $('#type').value = 'author';
    let author = atob(authorEle.dataset.author);
    $('#word').value = author;
    search('author', author);
};

var formatTitle = function(record) {
    return '<a href="dblp://'+ record.mdate + '/'+ record.key +'">' + record.title + '</a>';
};

var formatAuthor = function(author) {
    return '<span class="search-author-other" onclick="searchAuthor(this)" data-author="'+ btoa(author) +'">' + author + '</span>';
}

var formatAuthors = function(record) {
    let ref = record.authors;
    for(let j = 0; j < ref.length; ++j) {
        if($('#type') == 'title' || ref[j] != $('#word').value){
            ref[j] = formatAuthor(ref[j]);
        }else{
            ref[j] = '<span class="search-author">' + ref[j] + '</span>';
        }
    }
    return ref.join('; ');
}

var clearBefore = function() {
    $('#info').innerHTML = "";
    $('#thead').innerHTML = "";
    $('#tbody').innerHTML = "";
    $('#homepage').style.display = "none";
    $('#info').style.display = "none";
    $('#coGraph').style.display = "none";
}

var handleHomePage = function(record) {
    $('#homepage').href = 'dblp://' + record.mdate + '/' + record.key;
    metaText = $('#word').value;
    $('#homepage-meta').innerText = metaText;
    $('#homepage').style.display = "block";
}

var getNodes = function(parentNode,childNodes,nodes){
    var pnode;
    for(var i=0; i<nodes.length; i++){
        if(parentNode === nodes[i].nodename){
            pnode = nodes[i];
        }
    }
    for(i=0; i<childNodes.length; i++){
        var flag=false;
        var level=pnode.nodelevel;
        nodes.forEach(item=>{
            if(item.nodename === childNodes[i]){
            flag=true;
            level=item.nodelevel;
            }
        });
        if(flag){
            nodes.push({
                nodename : childNodes[i],
                nodelevel: level,
                parentnode: parentNode,
            });
        }
        else{
            nodes.push({
            nodename : childNodes[i],
            nodelevel: pnode.nodelevel+1,
            parentnode: parentNode,
            });
        }
    }
}

var setNodeData = function(nodes,listdata) {
    var size = 40;
    for(var i=0; i<nodes.length; i++){
        var flag=false;
        listdata.forEach(item=>{
            if(item.name === nodes[i].nodename){
                flag=true;
            }
        });
        if(!flag){
            listdata.push({
                category: nodes[i].nodelevel,
                name: nodes[i].nodename,
                symbolSize: size-nodes[i].nodelevel*10,
                draggable: "true",
                symbol:"circle"
            });
        }
    }
}

var setLinkData = function(childList, parentnode, links) {
    for(var i=0; i<childList.length; i++){
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
    clearBefore();

    if(data == "not_ready") return ;
    let json = JSON.parse(data);
//     console.log(json);
    if(json.length == 0){
        $('#info').innerHTML = tr("NOT FOUND!");
        $('#info').style.display = "block";
        return ;
    }

    if($('#type').value == 'coauthor') {

        $('#result').style.display='inline-block';
        $('#coGraph').style.display = "none";

        $('#thead').innerHTML = '<tr> <th> </th> <th>' + tr('Co-Author(s)') + '</th> </tr>';
        for(let i = 0; i < json.length; ++i){
            $('#tbody').innerHTML += '<tr><td>' + (i+1) + '</td><td>' + formatAuthor(json[i]) + '</td></tr>';
        }
    }else if($('#type').value == 'title') {

        $('#result').style.display='inline-block';
        $('#coGraph').style.display = "none";

        $('#thead').innerHTML = '<tr> <th> </th> <th>' + tr('Title') + '</th> <th>' + tr('Author(s)') + '</th> <th>' + tr('Modified') + '</th> </tr>';
        json.sort(function(x, y) {
            return parseInt(x.mdate) - parseInt(y.mdate);
        });
        for(let i = 0; i < json.length; ++i) {
            let tr = $('<tr>');
            tr.innerHTML += '<td>' + (i+1) + '</td>';
            tr.innerHTML += '<td>' + formatTitle(json[i]) + '</td>';
            tr.innerHTML += '<td>' + formatAuthors(json[i]) + '</td>';
            tr.innerHTML += '<td>' + json[i].mdate + '</td>';
            $('#tbody').appendChild(tr);
        }
    }else if($('#type').value == 'author') {

        $('#result').style.display='inline-block';
        $('#coGraph').style.display = "none";

        $('#thead').innerHTML = '<tr> <th> </th> <th>' + tr('Title') + '</th> <th>' + tr('Author(s)') + '</th> <th>' + tr('Year') + '</th> </tr>';
        json.sort(function(x, y) {
            return parseInt(x.year) - parseInt(y.year);
        });
        let label = 1;
        for(let i = 0; i < json.length; ++i){
            if(json[i].title == "Home Page"){
                handleHomePage(json[i]);
                continue;
            }
            let tr = $('<tr>');
            tr.innerHTML += '<td>' + label + '</td>';
            tr.innerHTML += '<td>' + formatTitle(json[i]) + '</td>';
            tr.innerHTML += '<td>' + formatAuthors(json[i]) + '</td>';
            tr.innerHTML += '<td>' + json[i].year + '</td>';
            $('#tbody').appendChild(tr);
            label = label + 1;
        }
    }else if($('#type').value == 'cograph'){

        $('#result').style.display = "none";
        $('#coGraph').style.display = "block";

//        var parent = document.getElementById("coGraph");
//        var saveButton = document.createElement('button');
//        parent.appendChild(saveButton);

//        saveButton.innerHTML = '保存';

        $("#save").onclick = function () {
            var myChart = echarts.getInstanceByDom(document.getElementById("graph"));
            var url = myChart.getDataURL();
            console.log(url);
            finder.image(url,json[0].parentNode);
//            var a = document.createElement('a');
//            var event = new MouseEvent('click');

//                a.download = json[0].parentNode||'default';
//                a.href = url;

//                a.dispatchEvent(event);
        };
//        parent.appendChild(saveButton);

        var myChart = echarts.init(document.getElementById('graph'));

        var listdata = [];
        var linksdata = [];

//        console.log(json[0].parentNode);

        var nodes =[{
                "nodename":json[0].parentNode,
                "nodelevel":0,
                "parentnode":0
            }];

        //handle data
        for(var i=0; i < json.length; i++){
            getNodes(json[i].parentNode,json[i].childNodes,nodes);
            setLinkData( json[i].childNodes, json[i].parentNode, linksdata);
        }
        setNodeData(nodes,listdata);

//        for(var i=0;i<nodes.length;i++){
//            console.log(i+"nodes: "+nodes[i].nodename+" "+nodes[i].nodelevel+" "+nodes[i].parentnode);
//        }
//        for(var i=0;i<listdata.length;i++){
//            console.log(i+"listdata: "+listdata[i].name+" "+listdata[i].category);
//        }
//        for(var i=0;i<linksdata.length;i++){
//            console.log(i+"linksdata: "+linksdata[i].target+" "+linksdata[i].source);
//        }

        var levels = 0;
        var legend_data = [];
        var series_categories = [];
        var temp = ["一","二","三","四","五"];
        for(var i=0; i < nodes.length; i++){
            levels = Math.max(levels, nodes[i].nodelevel);
        }
        for(var i=0; i<=levels; i++){

            legend_data.push({
                name : i===0?'父节点':'层级'+temp[i],
                icon : 'rect'
            });

            series_categories.push({
                name : i===0?'父节点':'层级'+temp[i],
                symbol : 'rect'
            });

        }

        option = {
            title: {
                text: json[0].parentNode + "の合作关系图",
                top: "top",
                left: "center",
                textStyle: {
                    color: '#292421',
                    fontSize: 25,
                }
            },
            tooltip: {
                formatter: '{b}'
            },
            toolbox: {
                show : true,
                feature : {
                    dataView: {readOnly: false},
                               restore: {},
//                               saveAsImage: {}
                }
            },
            backgroundColor: '#e7e7e7',
            legend: {
                show : true,
                data : legend_data,
                textStyle: {
                    color: '#292421'
                },
                selector: [
                    {
                        type: 'all or inverse',
                        title: '全选'
                    },
                    {
                        type: 'inverse',
                        title: '反选'
                    }
                ],
                backgroundColor:'#F8F8FF',
                shadowColor: 'rgba(0, 0, 0, 0.5)',
                shadowBlur: 10,
                icon: 'circle',
                type: 'scroll',
                orient: 'vertical',
                left: 10,
                top: 20,
                bottom: 20,
                itemWidth: 10,
                itemHeight: 10
            },
            animationDuration: 1500,
            animationEasingUpdate: 'quinticInOut',
            series: [{
                name: '关系图',
                type: 'graph',
                layout: 'circular',
                circular: {
                    rotateLabel: true
                },
//                force: {
//                    repulsion: 300,
//                    gravity: 0.1,
//                    edgeLength: 15,
//                    layoutAnimation: true,
//                },
                data: listdata,
                links: linksdata,
                categories: series_categories,
                roam: true,
                focusNodeAdjacency: true,
                emphasis: {
                   itemStyle: {
                       shadowBlur: 10,
                       shadowOffsetX: 0,
                       shadowColor: 'rgba(0, 0, 0, 0.5)'
                       }
                },
                label: {
                    normal: {
                        show: true,
                        position: 'bottom',
                        formatter: '{b}',
                        fontSize: 10,
                        fontStyle: '600',
                    }
                },
                itemStyle: {
                            borderColor: '#fff',
                            borderWidth: 1,
                            shadowBlur: 10,
                            shadowColor: 'rgba(0, 0, 0, 0.3)'
                        },
                lineStyle: {
                    normal: {

                        opacity: 0.9,
                        width: 0.5,
                        curveness: 0
                    }
                },
                emphasis: {
                    lineStyle: {
                        idth: 10
                    }
                }
            }]
        };

        myChart.setOption(option);

        var first=$("#graph").firstChild.style.margin='0 auto';

        var data=myChart._model.option.series[0].data;
           myChart.on("click", function (chartParam){
                console.log(chartParam);
               if(chartParam.borderColor){
                   $('#word').value=data[chartParam.dataIndex].name;
                   search($('#type').value, $('#word').value);
               }
           });
    }
};

var resSaveMes= function(data){
    if(data){
        alert("保存成功，请前往索引文件目录查看");
    }
    else{
        alert("保存失败！");
    }
}

if(typeof QWebChannel != "undefined"){
    new QWebChannel(qt.webChannelTransport, function(channel) {
        finder = channel.objects.finder;
        finder.ready.connect(handleSearch);
        finder.saveImg.connect(resSaveMes);
    });
}

var handleInput = function() {
    search($('#type').value, $('#word').value);
}

$('#search').onclick = handleInput;
$('#word').addEventListener('keydown', function(e) {
    if(e.keyCode == 13) handleInput();
});

$('#word').focus();

if(location.href.startsWith('file:')) {
    $.load('index.test.js', function() {
        // test.coauthor;
    });
}
