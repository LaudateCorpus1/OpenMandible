@echo off
del tetgen-files\*.*
rmdir tetgen-files
mkdir tetgen-files
COPY  stl\*.* tetgen-files
COPY  tetgen_win64.exe tetgen-files
cd tetgen-files
for %%f in (*.stl) do (tetgen_win64.exe -Y -q10.0 %%f)
DEL  *.edge
DEL  *.smesh
DEL  *.face
DEL  *.stl
DEL tetgen_win64.exe
cd ..