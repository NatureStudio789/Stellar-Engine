@echo off

set /p IDE=Please Input Your IDE:

call "Dependencies/Binaries/Premake/premake5.exe" --file=Stellar-Engine.lua %IDE%

pause >nul
