@echo off
set file=DBLParse.qrc
echo ^<RCC^>^<qresource prefix="/"^> > %file%
for %%i in (resources\*) do echo ^<file^>%%i^</file^> >> %file% 
echo ^</qresource^>^</RCC^> >> %file% 