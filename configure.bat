@echo off
cmd /C "cmake -B .\build -D CMAKE_BUILD_TYPE=Debug && xcopy /D /Y .\build\compile_commands.json .\"
