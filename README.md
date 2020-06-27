<h1 align="center">DBLParse</h1>
<p align="center">
  <a href="https://github.com/tootal/DBLParse/releases">
    <img alt="Github Release" src="https://img.shields.io/github/release/tootal/DBLParse.svg" target="_blank" />
  </a>
  <a href="https://github.com/tootal/DBLParse/actions?query=workflow%3AWindows">
    <img alt="Github Actions (Windows)" src="https://github.com/tootal/DBLParse/workflows/Windows/badge.svg" target="_blank" />
  </a>
  <a href="https://github.com/tootal/DBLParse/actions?query=workflow%3AUbuntu">
    <img alt="Github Actions (Ubuntu)" src="https://github.com/tootal/DBLParse/workflows/Ubuntu/badge.svg" target="_blank" />
  </a>
</p>

DBLParse是基于DBLP大数据，利用Web和Qt建立的一套高效率的文献查询与分析系统。该系统能够根据作者名，论文题目，或者论文中的关键词来查询符合条件的论文信息，也能够通过作者名来查询与其有合作关系的作者，并通过图像的方式形象地展现合作作者关系。基于这些信息，系统还为用户提供了强大的分析与统计功能，能够从大量的DBLP数据中统计出写文章数前100的作者，分析每一年发表的文章题目中的热点词汇并生成词云，统计整个数据集里作者的合作关系图中各阶完全子图的个数，为用户深入透彻地分析论文数据提供了便利。总的来说，本系统能够帮助科研工作者快速获取和分析现有的文献，提高科学文献的管理效率，了解国外学术研究的方向。

## 安装
### Windows
支持版本：Windows 7 x64、Windows 10 x64

* 下载最新发布的压缩包DBLParse.7z，链接：https://github.com/tootal/DBLParse/releases
* 使用主流解压缩软件解压到单独的文件夹下
* 双击文件夹下的可执行文件DBLParse.exe即可运行
* 若出现错误可尝试运行文件夹下的vc_redist.x64.exe安装运行环境

### Ubuntu
支持版本：Ubuntu 18.04，Ubuntu 20.04

* 下载最新发布的镜像文件DBLParse.AppImage，链接：https://github.com/tootal/DBLParse/releases
* 双击下载好的DBLParse.AppImage即可运行

## 主要功能

* 基本搜索功能。输入作者名，能展示该作者发表的所有论文信息。输入完整的论文的题目，能展示该论文的其他相关信息。
* 相关搜索。输入作者名，能展示于该作者有合作关系的其他所有作者。
* 作者统计功能。输出写文章最多的前100名作者。
* 热点分析功能。分析每一年发表的文章中，题目所包含的单词中，出现频率排名前10的关键词。
* 部分匹配搜索功能。给定若干个关键字，能快速搜索到题目中包含该关键字的文章信息
* 聚团分析。作者之间的合作关系可以看成是一个图，每个作者对应一个顶点，任两个作者之间如果存在合作关系，则在两个顶点之间建立连边。这个图中的每一个完全子图我们称为一个聚团（所谓完全子图指的是该子图的任意顶点都和该子图的其他顶点有连边，完全子图的顶点个数称为该完全子图的阶数），统计整个图中各阶完全子图的个数。
* 可视化显示。通过图形化界面，展示作者之间合作关系图及其相关文章信息。

## 相关资料

* [更新日志](docs/CHANGES.md)
* [开发文档](docs/CONTRIBUTING.md)
* [关于DBLP数据](docs/DBLPXML.md)
* [任务书](docs/TASKBOOK.md)

## 依赖

* [DBLP XML数据文件](https://dblp.org/xml/), (CC0 1.0)
* [Qt开源版](https://doc.qt.io/qt-5/licensing.html), (L-GPL v3)
* [Qt WebEngine模块](https://doc.qt.io/qt-5/qtwebengine-licensing.html), (L-GPL v3)
* [C++大整数计算类](https://github.com/Limeoats/BigNumber), (MIT)
* [Pivoter各阶完全子图精确计数](https://bitbucket.org/sjain12/pivoter/src/master/), (GPL 3.0)
* [Vue 渐进式JavaScript框架](https://cn.vuejs.org/index.html), (MIT)
* [Bootstrap响应式框架](https://getbootstrap.com/), (MIT)
* [Bootstrap-select](https://github.com/snapappointments/bootstrap-select), (MIT)
* [ECharts开源可视化库](https://echarts.apache.org/zh/index.html), (Apache-2.0)
* [echarts-wordcloud 基于wordcloud2.js的词云扩展库](https://github.com/ecomfe/echarts-wordcloud)

## 许可
DBLParse使用GPL 3.0许可。