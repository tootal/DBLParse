'Setting up environment...'
$env:path="C:\Qt\Qt5.12.7\5.12.7\msvc2017_64\bin;"+$evn:path
qmake --version
$env:VCINSTALLDIR="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC"

'Removing cache...'
Set-Location ..
Remove-Item -Recurse deploy

'Building...'
qmake
nmake 

'Deploying...'
New-Item -ItemType Directory deploy 
Set-Location deploy 
New-Item -ItemType Directory DBLParse 
Set-Location .. 
Copy-Item src\release\DBLParse.exe deploy\DBLParse 
Set-Location deploy\DBLParse 
windeployqt DBLParse.exe 

'Compressing...'
Set-Location ..
Compress-Archive -Path DBLParse DBLParse.zip

'Clean...'
Remove-Item -Recurse pivoter\debug
Remove-Item -Recurse pivoter\debug
Remove-Item -Recurse pivoter\debug
Remove-Item -Recurse pivoter\debug
Remove-Item -Recurse pivoter\debug
Remove-Item -Recurse pivoter\debug
Remove-Item .qmake.stash
Remove-Item Makefile*

'Deploy successful!'
start deploy