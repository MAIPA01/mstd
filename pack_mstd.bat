@echo off
setlocal enabledelayedexpansion

:: --- DEFAULT PATHS CONFIG ---
:: Python script path
SET SCRIPT_PATH=".\tools\pack_mstd\pack_mstd.py"

:: Default resource directory
SET SOURCE_DIR=".\mstd\include"

:: Default output directory
SET OUTPUT_DIR=".\package"
:: ---------------------------------------

echo ========================================
echo   Pack 'mstd' resources to ZIP
echo ========================================
echo.

:: Ask user to provide version number
set /p VERSION="Provide version number (example: 1.0.2): "

:: Check if version number wasn't empty
if "%VERSION%"=="" (
    echo.
    echo [ERROR] Version number wasn't provided! Operation canceled.
    pause
    exit /b
)

echo.
echo [INFO] Starting Python script...
echo [INFO] Source:     %SOURCE_DIR%
echo [INFO] Output:     %OUTPUT_DIR%
echo [INFO] Version:    %VERSION%
echo.

:: Run python script with provided arguments
python %SCRIPT_PATH% %SOURCE_DIR% %OUTPUT_DIR% %VERSION%

:: Check python ERRORLEVEL
:: 0 is a success, everything greater is error
if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Packing finished with success.
    echo [INFO] Opening output directory...
    
    :: Output directory opened in new window
    start "" %OUTPUT_DIR%
) else (
    echo.
    echo [ERROR] Python script returned error. Directory will not open.
)

echo.
echo Press any key to close this window.
pause >nul