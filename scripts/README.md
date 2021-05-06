# 构建脚本

此文件夹存放构建软件过程中使用的部分脚本。  
关于Github Action自动化构建的脚本放在/.github/workflows文件夹下。  

## Windows

命令行执行过程：

```cmd
Windows PowerShell
版权所有 (C) Microsoft Corporation。保留所有权利。

尝试新的跨平台 PowerShell https://aka.ms/pscore6

PS C:\Users\huangzhiquan> cmd
Microsoft Windows [版本 10.0.19042.928]
(c) Microsoft Corporation。保留所有权利。

C:\Users\huangzhiquan>rmdir /?
删除一个目录。

RMDIR [/S] [/Q] [drive:]path
RD [/S] [/Q] [drive:]path

    /S      除目录本身外，还将删除指定目录下的所有子目录和
            文件。用于删除目录树。

    /Q      安静模式，带 /S 删除目录树时不要求确认

C:\Users\huangzhiquan>rmdir /s/q build
系统找不到指定的文件。

C:\Users\huangzhiquan>cd DBLParse

C:\Users\huangzhiquan\DBLParse>rmdir /s/q build

C:\Users\huangzhiquan\DBLParse>cmake -B build
-- Building for: Visual Studio 16 2019
-- Selecting Windows SDK version 10.0.19041.0 to target Windows 10.0.19042.
-- The C compiler identification is MSVC 19.28.29914.0
-- The CXX compiler identification is MSVC 19.28.29914.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/Enterprise/VC/Tools/MSVC/14.28.29910/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/Enterprise/VC/Tools/MSVC/14.28.29910/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/huangzhiquan/DBLParse/build

C:\Users\huangzhiquan\DBLParse>cmake --build build --config Release
用于 .NET Framework 的 Microsoft (R) 生成引擎版本 16.9.0+5e4b48a27
版权所有(C) Microsoft Corporation。保留所有权利。

  Checking Build System
  Building Custom Rule C:/Users/huangzhiquan/DBLParse/bignumber/CMakeLists.txt
  bignumber.cpp
  BigNumber.vcxproj -> C:\Users\huangzhiquan\DBLParse\build\bignumber\Release\BigNumber.lib
  Automatic MOC for target Pivoter
  Building Custom Rule C:/Users/huangzhiquan/DBLParse/pivoter/CMakeLists.txt
  LinkedList.cpp
  degeneracy_algorithm_cliques_A.cpp
  degeneracy_helper.cpp
  misc.cpp
  mocs_compilation_Release.cpp
  正在生成代码...
  Pivoter.vcxproj -> C:\Users\huangzhiquan\DBLParse\build\pivoter\Release\Pivoter.lib
  Automatic MOC and UIC for target DBLParse
  Automatic RCC for DBLParse.qrc
  Building Custom Rule C:/Users/huangzhiquan/DBLParse/src/CMakeLists.txt
  application.cpp
  configmanager.cpp
  downloaddialog.cpp
  parsedialog.cpp
  settingsdialog.cpp
  statusdialog.cpp
  finder.cpp
  loader.cpp
  main.cpp
  mainwindow.cpp
  networker.cpp
  parser.cpp
  record.cpp
  stemmer.cpp
  util.cpp
  webpage.cpp
  webview.cpp
  statuslabel.cpp
  qrc_DBLParse.cpp
  mocs_compilation_Release.cpp
  正在生成代码...
  DBLParse.vcxproj -> C:\Users\huangzhiquan\DBLParse\build\src\Release\DBLParse.exe
  Building Custom Rule C:/Users/huangzhiquan/DBLParse/CMakeLists.txt

C:\Users\huangzhiquan\DBLParse>mkdir deploy\DBLParse

C:\Users\huangzhiquan\DBLParse>copy build\src\Release\DBLParse.exe deploy\DBLParse
已复制         1 个文件。


```