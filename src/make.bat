cl /Fe..\bin\tint.exe tint.cc *.cpp ..\deps\sand\sand.cpp ..\deps\dot\dot.cpp ..\deps\sao\file.cpp ..\deps\sao\sao.cpp ..\deps\wire\wire.cpp /EHsc /Ox /Oy /MT -I ..\deps
\prj\tools\upx ..\bin\*.exe
run