C:\Qt\4.8.7\bin\rcc.exe -binary resource/resource.qrc -o dist/resource.rcc
mkdir .\dist\lang\
copy /Y .\resource\languages\*.ini .\dist\lang\
pause