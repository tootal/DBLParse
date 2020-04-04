# 开发文档
## 开发环境
操作系统：Windows 10 64位  
IDE：Qt Creator  
编译器：Qt 5.12.7 MSVC2017 64bit  

## 名称规范
前缀约定：  
全局变量：`g_`  
常量：`c_`  
成员变量：`m_`  
静态成员变量：`s_`  

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
引入前端框架[Bootstrap](https://getbootstrap.com/)的样式文件（`bootstrap.min.css`，版本为4.4.1，注意没有引入JS文件。


## 前后端交互
### WebView
临时创建一个Web窗口可以使用`QWebEngineView`，利用`load`、`setUrl`或`setHtml`加载数据。若要自定义一些功能（例如拦截URL、自定义右键菜单）可以继承`QWebEngineView`和`QWebEnginePage`类。具体实现可以参考`WebView`和`WebPage`。

### WebChannel
若是需要JS和C++通信，可以加入`QWebChannel`。C++端的`WebPage`使用`setWebChannel`指定`WebChannel`，接着使用`registerObject`注册C++类（必须继承QObject），同时在希望被JS端调用的函数前加上宏定义`Q_INVOKABLE`；在HTML文件中引入`qwebchannel.js`文件（在`body`标签末尾加入`<script src="qwebchannel.js"></script>`），在脚本中引入已经注册的C++类、连接QT信号到JS函数。例如：

```js
new QWebChannel(qt.webChannelTransport, function(channel) {
    finder = channel.objects.finder;
    finder.ready.connect(handleSearch);
});
```

其中`ready`是C++类`Finder`的一个信号，`handleSearch`是一个JS定义的函数。

利用JS调用C++函数（通过函数参数传递信息）以及JS端函数绑定信号（利用信号传递信息到JS函数参数中）可以实现前后端的双向交互。

## 前端测试
为了方便测试前端效果，在JS文件中做了特殊的处理，使得前端文件支持本地打开（利用`location.href`判断）。在浏览器中打开HTML文件会自动加载对应的测试文件`*.test.js`。  

目前支持的本地测试数据记录如下：  
作者名：`Katja Lenz`  
标题：`The Conjunctive Complexity of Quadratic Boolean Functions.`  
禁用标题：`Home Page`

以上数据定义在`index.test.js`文件中。

## 发布

（使用bandizip压缩命令）

* 在QT Creator中在Release模式下编译程序，确保exe文件正确生成。
* 打开到源代码目录，双击`deploy.bat`脚本即可自动发布。
* 生成完成后会自动打开exe文件所在目录，其中`DBLParse.zip`和DBLParse文件夹为构建好的文件。
