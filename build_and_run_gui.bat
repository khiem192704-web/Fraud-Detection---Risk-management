@echo off
setlocal
echo ========================================================
echo   EPFD-RAS: BIEN DICH VA KHOI CHAY GIAO DIEN QT GUI
echo ========================================================

set "PATH=C:\QT\6.11.2\mingw_64\bin;C:\QT\Tools\mingw1310_64\bin;C:\Program Files\CMake\bin;%PATH%"

cd /d "%~dp0"

if not exist "build_gui" mkdir "build_gui"

echo [1/3] Dang cau hinh target epfd_gui voi CMake...
cmake -B build_gui -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/QT/6.11.2/mingw_64"

echo [2/3] Dang bien dich epfd_gui...
cmake --build build_gui --target epfd_gui

if %ERRORLEVEL% equ 0 (
    echo.
    echo [3/3] Bien dich thanh cong! Dang dong goi thu vien Qt DLLs...
    windeployqt --no-translations --no-compiler-runtime "build_gui\epfd_gui.exe" >nul 2>&1
    echo.
    echo Khoi chay epfd_gui.exe...
    start "" "%~dp0build_gui\epfd_gui.exe"
) else (
    echo.
    echo [LOI] Bien dich that bai! Vui long kiem tra thong bao loi o tren.
)

endlocal
