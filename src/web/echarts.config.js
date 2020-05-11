var option = {
    title: {
        text: "defaultの合作关系图",
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
        show: true,
        feature: {
            dataView: { readOnly: false },
            restore: {},
            //saveAsImage: {}
        }
    },
    backgroundColor: '#e7e7e7',
    legend: {
        show: true,
        data: '',
        textStyle: {
            color: '#292421'
        },
        selector: [{
                type: 'all or inverse',
                title: '全选'
            },
            {
                type: 'inverse',
                title: '反选'
            }
        ],
        backgroundColor: '#F8F8FF',
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
        data: '',
        links: '',
        categories: '',
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
