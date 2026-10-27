@echo off
title Smart Traffic Light System - 3D OpenGL Simulation
echo ==============================================================================
echo  SMART TRAFFIC LIGHT SYSTEM WITH EMERGENCY VEHICLE PRIORITY (3D OPENGL)
echo ==============================================================================
echo.
echo Launching 3D Four-Way Traffic Simulation...
echo.
echo CONTROLS:
echo   [E]          : Dispatch Ambulance (Flashing blue beacon + priority wave)
echo   [F]          : Dispatch Fire Truck
echo   [SPACE]      : Pause / Resume simulation
echo   [S]          : Manually advance traffic signal phase
echo   [R]          : Reset simulation and vehicles
echo   [Arrow Keys] : Orbit camera around junction
echo   [+ / -]      : Zoom In / Zoom Out
echo   [0]          : Reset camera to default elevated perspective
echo   [V]          : Switch View (3D Perspective / 2D Overhead / Driver Cam)
echo   [ESC]        : Exit
echo.
if exist "CG_OpenGL_3D\traffic_3d.exe" (
    start "" "CG_OpenGL_3D\traffic_3d.exe"
) else if exist "traffic_3d.exe" (
    start "" "traffic_3d.exe"
) else (
    echo [ERROR] traffic_3d.exe not found!
    echo Compiling with g++...
    g++ -O2 CG_OpenGL_3D\traffic_3d.cpp -o CG_OpenGL_3D\traffic_3d.exe -lfreeglut -lopengl32 -lglu32
    if %ERRORLEVEL% EQU 0 (
        echo Compilation successful! Launching...
        start "" "CG_OpenGL_3D\traffic_3d.exe"
    ) else (
        echo Compilation failed. Ensure MinGW FreeGLUT is installed.
        pause
    )
)
