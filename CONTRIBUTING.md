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
选择Qt Creator菜单栏Tools，External，Qt预言家。首先用lupdate更新翻译文件（导出tr字符串）。接着打开linguist（QT语言家），打开ts文件，选中对应字符串进行翻译。翻译完成后，在Qt Creator中用lrelease生成qm文件。


## 发布

* 在QT Creator中用Release编译程序，把可执行文件复制到新文件夹下
* 启动QT 5.12.7 （MSVC 2017 64-bit）命令行
* 切换到可执行文件目录下，执行`windeployqt DBLParse.exe`