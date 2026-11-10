@echo off
cmd /C "cmake --build build --config Debug && xcopy /D /Y .\build\vendor\raylib\raylib\raylib.dll .\build && xcopy /D /Y .\build\vendor\raylib\SDL\SDL3.dll .\build"
