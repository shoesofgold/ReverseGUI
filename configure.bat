@echo off
cmd /C "cmake -B .\build && xcopy /D /Y .\build\compile_commands.json .\"
