@echo off
REM Build Loader.exe - run from Loader folder or cheese root

cd /d "%~dp0"

if exist "Loader.sln" (
    echo Building with MSBuild...
    msbuild Loader.sln /p:Configuration=Release /p:Platform=x64 /v:minimal
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo Loader.exe built successfully in parent folder.
    ) else (
        echo Build failed.
        exit /b 1
    )
) else (
    echo Loader.sln not found. Use Visual Studio or: cmake -B build -A x64 && cmake --build build --config Release
    exit /b 1
)
