if "%CMAKE_GENERATOR%" == "" (
    echo You should set the variable CMAKE_GENERATOR before building
    exit 1
)

mkdir build
cd build

REM Configure step
cmake -G "%CMAKE_GENERATOR%" -DCMAKE_PREFIX_PATH=%LIBRARY_PREFIX% -DCMAKE_INSTALL_PREFIX:PATH=%LIBRARY_PREFIX% %SRC_DIR% -DPY_DEBUG=True
if errorlevel 1 exit 1

REM Build step
cmake --build . --config Debug --target INSTALL
if errorlevel 1 exit 1
