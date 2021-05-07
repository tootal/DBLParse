# 构建脚本

此文件夹存放构建软件过程中使用的部分脚本。  
关于Github Action自动化构建的脚本放在`/.github/workflows`文件夹下。  

构建脚本（包含环境配置、编译、打包）

* Windows：win_build.bat，可以双击执行或通过命令行调用，产物保存在`deploy/DBLParse.zip`。
* Ubuntu：unx_build.sh，添加执行权限后通过命令行调用，产物保存在`deploy/DBLParse.AppImage`。
* macOS：mac_build.sh，添加执行权限后通过命令行调用，产物保存在`deploy/DBLParse.dmg`。

