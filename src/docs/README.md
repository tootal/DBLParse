<h1 align="center">DBLParse</h1>

DBLParse is a set of high-efficiency document query and analysis system based on dblp computer science bibliography, using Web and Qt. The system can query the qualified paper information according to the author's name, the title of the paper, or the keywords in the title, and can also query the authors who have a cooperative relationship with the author by the author's name, and vividly show the cooperative author relationship through images. Based on this information, the system also provides users with powerful analysis and statistics functions. It can count the top-100 authors of articles from a large amount of DBLP data, analyze the hot vocabulary in the article titles published each year and generate a word cloud , Count the number of complete subgraphs of each order in the author’s cooperative relationship graph in the entire data set, which provides convenience for users to analyze the paper data thoroughly. In general, this system can help scientific researchers quickly obtain and analyze existing literature, improve the management efficiency of scientific literature, and understand the direction of foreign academic research.

## Installation
### Windows
Supported version: Windows 7 x64, Windows 10 x64

* [Download the newly released](https://github.com/tootal/DBLParse/releases) compressed package `DBLParse.zip`
* Extract to a separate folder
* Double-click the executable file `DBLParse.exe` to run
* Run the `vc_redist.x64.exe` when an error occurs

### Ubuntu
Supported version: Ubuntu 18.04, Ubuntu 20.04

* [Download the latest released](https://github.com/tootal/DBLParse/releases) appimage file `DBLParse.AppImage`
* Make this file executable
* Double-click the `DBLParse.AppImage` to run

### MacOS
**Warning: MacOS package has been released but not been tested.**

* [Download the latest released](https://github.com/tootal/DBLParse/releases) file `DBLParse.dmg`
* Install and run

Data configuration instructions:

The software needs additional DBLP data files to run. You can click the `Tools` menu in the menu bar and select the `Download Data` menu item.
If you select Yes in the pop-up dialog box, the browser will automatically open to download the data file. If the download link is unavailable or the download is too slow, you can switch to other download links.
After the download is complete, the file `dblp.xml.gz` is obtained. Decompress the file to obtain the data file `dblp.xml`.
Drag it and drop in main windows of system, the process of parse will automatically begin.

## The main function

* Basic search function. Enter the name of the author to display all the papers published by the author. Enter the title of the complete paper, which can display other relevant information about the paper.
* related search. Enter the name of the author to show all other authors who have a cooperative relationship with the author.
* Author statistics function. Output the top 100 authors who wrote the most articles.
* Hot spot analysis function. Analyze the top 10 keywords that appear frequently among the words contained in the titles of articles published each year.
* Partial match search function. Given several keywords, you can quickly search for the article information that contains the keyword in the title
* Cluster analysis. The cooperative relationship between authors can be regarded as a graph, and each author corresponds to a vertex. If there is a cooperative relationship between any two authors, an edge is established between the two vertices. Each complete subgraph in this graph is called a cluster (the so-called complete subgraph means that any vertex of the subgraph is connected to other vertices of the subgraph, and the number of vertices of the complete subgraph is called The order of the complete subgraph), count the number of complete subgraphs of each order in the entire graph.
* Visual display. Through a graphical interface, a diagram of the collaboration between authors and related article information is displayed.

## Dependence

* [DBLP XML data file](https://dblp.org/xml/), (CC0 1.0)
* [Qt Open Source Edition](https://doc.qt.io/qt-5/licensing.html), (L-GPL v3)
* [Qt WebEngine Module](https://doc.qt.io/qt-5/qtwebengine-licensing.html), (L-GPL v3)
* [C++ Big Integer Calculation Class](https://github.com/Limeoats/BigNumber), (MIT)
* [Pivoter exact clique count](https://bitbucket.org/sjain12/pivoter/src/master/), (GPL 3.0)
* [Vue Progressive JavaScript Framework](https://cn.vuejs.org/index.html), (MIT)
* [Bootstrap Responsive Framework](https://getbootstrap.com/), (MIT)
* [Bootstrap-select](https://github.com/snapappointments/bootstrap-select), (MIT)
* [ECharts Open Source Visualization Library](https://echarts.apache.org/zh/index.html), (Apache-2.0)
* [echarts-wordcloud word cloud extension library based on wordcloud2.js](https://github.com/ecomfe/echarts-wordcloud)

## Permission
DBLParse is licensed under the GPL 3.0 license.