del /F /S /Q .\publish\logs
del /F /S /Q .\publish\cnf
del /F /S /Q .\publish\ZKSort.exe

mkdir .\publish\logs
mkdir .\publish\cnf

copy .\dist\*.rcc .\publish\
copy .\dist\students.db .\publish\
copy .\dist\ZKSort.exe .\publish
copy .\dist\appsetting.ini .\publish
