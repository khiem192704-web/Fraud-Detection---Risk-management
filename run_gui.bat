@echo off
set "PATH=C:\QT\6.11.2\mingw_64\bin;C:\QT\Tools\mingw1310_64\bin;%PATH%"
cd /d "%~dp0"
if exist "build_gui\epfd_gui.exe" (
    start "" "%~dp0build_gui\epfd_gui.exe"
) else (
    echo [Loi] Chua tim thay build_gui\epfd_gui.exe! Vui long chay build_and_run_gui.bat de bien dich truoc.
    pause
)
