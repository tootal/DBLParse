# 开发文档
## 开发环境
操作系统：Windows 10 64位  
IDE：Qt Creator  
编译器：Qt 5.12.7 MSVC2017 64bit  

## 命名风格
类名称：单词首字母大写  
文件名：全部小写  
普通变量、函数名：首字母小写、其余单词首字母大写  
全局变量：`g_`    
常量：`c_`  
成员变量：`m_`  
静态成员变量：`s_`  

## 整体框架
整体架构分为前端（Web）和后端（QT），首先需要完整解析一遍`dblp.xml`文件，耗时约2分钟。解析部分在C++类`Parser`中定义，解析时显示信息的窗口使用`ParseDialog`类定义。解析完成后会生成若干数据文件（以`dat`结尾），其中保存了排序后的索引信息，运行时需要将这些文件加载到内存中。加载部分使用`Loader`类定义，加载完成后会把数据存在`Finder`类的静态变量中。查找时使用二分查找算法（`equalRange`）获取到对应关键字的位置，再从`dblp.xml`文件中读取出‘记录’信息（`Util::findRecord`），其中‘记录’对应的类为`Record`，利用`Record::attr`函数可以访问对应标签的内容，使用`Record::toJson`可以把`Record`转换成一个Json对象（`QJsonObject`）。

解析部分会在单独的线程中运行，具体来说就是先把`dblp.xml`文件一次性读入内存（占用约3G内存）中，接着使用`while`循环遍历一遍所有数据，遇到感兴趣的内容（如`<author`、`<title`、`key="`）就保存下该内容及其位置（生成索引）。接着按照字符串排序（调用`std::sort`），把排序好的数组保存在文件中。为了避免大量字符串复制带来的内存开销，使用`Parser::StringRef`内部类来保存字符串，该类的功能相当于`QStringRef`（但是`QStringRef`不支持超过2G的字符串）。

## 本地化
### 预处理
对于C++（QT）代码文件中需要翻译的字符串，使用`QObject::tr`函数包裹起来。例如：`statusBar()->showMessage(tr("Load finished."), 3000);`。  
对于UI文件中的字符串，无需特殊处理。  
对于HTML文件中的字符串，请在对应文本标签中加入自定义（最小化）属性`tr`，若无对应标签，请自行使用`span`标签包裹。例如：`<option value="author" tr>Author</option>`。  
对于JS文件中的字符串，请使用全局函数`tr`包裹起来。例如：`alert('warning', tr('You can not search this title.'));`。  

### 翻译
在Qt Creator的菜单栏中选择Tools，External，Qt语言家。首先用lupdate更新翻译文件（导出tr字符串到ts文件）。接着打开linguist（QT语言家），打开对应的ts文件，选中对应字符串进行翻译。翻译完成后，在Qt Creator中用lrelease生成qm文件。

对于HTML和JS中的字符串，在`strings_`开头的语言文件（js文件）中添加字符串映射即可。

## 资源文件
`resources`文件夹下的文件会全部加入到QT资源文件（`DBLParse.qrc`）中，可以使用批处理文件`resources.bat`文件生成`DBLParse.qrc`文件。

## 界面设计
### 后端
后端使用Qt Creator内置的UI设计师进行界面设计。

### 前端
引入前端框架[Bootstrap](https://getbootstrap.com/)进行布局。  

## 前后端交互
### WebView
临时创建一个Web窗口可以使用`QWebEngineView`，利用`load`、`setUrl`或`setHtml`加载数据。若要自定义一些功能（例如拦截URL、自定义右键菜单）可以继承`QWebEngineView`和`QWebEnginePage`类。具体实现可以参考`WebView`和`WebPage`类。

### WebChannel
若是需要JS和C++通信，可以加入`QWebChannel`。C++端的`WebPage`使用`setWebChannel`指定`WebChannel`，接着使用`registerObject`注册C++类（必须继承QObject），同时在希望被JS端调用的函数前加上宏定义`Q_INVOKABLE`；在HTML文件中引入`qwebchannel.js`文件（在`body`标签末尾加入`<script src="qwebchannel.js"></script>`），在脚本中引入已经注册的C++类、连接QT信号到JS函数。例如：

```js
new QWebChannel(qt.webChannelTransport, function(channel) {
    finder = channel.objects.finder;
    finder.ready.connect(handleSearch);
});
```

其中`ready`是C++类`Finder`的一个信号，`handleSearch`是一个JS定义的函数。此后可以在其他地方调用`finder`，例如执行`finder.find(type, word)`实质上会调用C++端的`Finder::find`函数。

利用JS调用C++函数（通过函数参数传递信息）以及JS端函数绑定信号（利用信号传递信息到JS函数参数中）可以实现前后端的双向交互。  

注意：如果不需要动态传输数据，只需要一次性传输数据（C++传到Web）可以使用字符串替换功能处理HTML文件，具体实现参考`DetailView`的实现。

## 前端测试
为了方便测试前端效果，在JS文件中做了特殊的处理，使得前端文件支持本地打开（利用`location.href`判断）。在浏览器中打开HTML文件会自动加载对应的测试文件`*.test.js`。    

目前支持的本地测试数据记录如下：    
作者名：`Katja Lenz`    
标题：`The Conjunctive Complexity of Quadratic Boolean Functions.`    
禁用标题：`Home Page`  

完整的测试数据和测试命令可查看`index.test.js`文件。

以下是常见的测试方式：  
* 直接输入测试数据。
* 在浏览器控制台中输入 `test.author`、`test.title`、`test.homepage`等命令，测试对应结果的样式。  
* 在浏览器控制台中输入`language='en'`、`language='zh'`测试翻译是否正确。  
* 在JS文件中加入上述命令来自动执行。

## 发布

（使用bandizip压缩命令）

* 在QT Creator中在Release模式下编译程序，确保exe文件正确生成。
* 打开到源代码目录，双击`deploy.bat`脚本即可自动发布。
* 生成完成后会自动打开exe文件所在目录，其中`DBLParse.zip`和DBLParse文件夹为构建好的文件。