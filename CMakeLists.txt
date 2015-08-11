# Options:
#
# [optional] - COMPILE_TESTS: Will compile the 'pyboost_ipc_tests' c++ module too. This module is necessary to run the tests of the project.
# 

cmake_minimum_required(VERSION 3.0)

project(pyboost_ipc)

find_package(Boost REQUIRED COMPONENTS python)

find_package(PythonLibs)
find_package(PythonInterp REQUIRED)

if (NOT ${PYTHONLIBS_FOUND})
    # attempt to find python if 'python.exe' is found
    message(STATUS "Trying to use the 'libs' and 'Include' directories at the python interpreter path")
    if (NOT ${PYTHONINTERP_FOUND})
        message(FATAL_ERROR "Didn't find the python libraries nor a python interpreter. You can pass the variables:
                             -DPYTHON_LIBRARY=<python_path>/libs
                             -DPYTHON_INCLUDE_DIR=<python_path>/Include
                             instead.
                             Make sure that the Python.h and the static libraries are installed (conda install libpython)
                             "
               )
    endif()

    get_filename_component(PYTHONINTERP_PATH ${PYTHON_EXECUTABLE} DIRECTORY)
    set(PYTHON_LIBRARIES ${PYTHONINTERP_PATH}/libs)
    execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_inc; print get_python_inc(True)" OUTPUT_VARIABLE PYTHON_INCLUDE_DIRS OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

# get the 'site-packages' directory
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(True)" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
    source/c++
)

add_definitions(
    -DBOOST_ALL_NO_LIB  # since we link the libraries using cmake we disable auto-linking
)

# Linking with Python for MSVC. I have to use link_directories() because the result of PYTHON_LIBRARIES is a directory.
# Also, the link_directories() command must be done before the target.
if (${MSVC})
    link_directories(${PYTHON_LIBRARIES})
endif()

# TARGET=pyboost_ipc
file(GLOB_RECURSE SOURCES source/c++/pyboost_ipc/*.*)
python_add_module(pyboost_ipc "${SOURCES}")
target_link_libraries(pyboost_ipc ${Boost_LIBRARIES})
if (UNIX)
    target_link_libraries(pyboost_ipc rt)
endif()

# TARGET=pyboost_ipc_tests
python_add_module(pyboost_ipc_tests source/c++/pyboost_ipc_tests/_pyboost_ipc_tests.cpp)
target_link_libraries(pyboost_ipc_tests ${Boost_LIBRARIES})

# Tests
option(COMPILE_TESTS "Compile test modules (necessary to run the python tests)" false)
if (${COMPILE_TESTS})
    message(STATUS "Will compile tests too")
    enable_testing()
    add_test(NAME python_tests
        COMMAND py.test ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

# Install
message(STATUS "Will install to: ${PYTHON_SITE_PACKAGES}")
install(TARGETS pyboost_ipc
    RUNTIME DESTINATION ${PYTHON_SITE_PACKAGES}
    LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES}
)

if (${COMPILE_TESTS})
    install(TARGETS pyboost_ipc_tests
        RUNTIME DESTINATION ${PYTHON_SITE_PACKAGES}
        LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES}
    )
endif()

install(FILES
    source/c++/pyboost_ipc/api.hpp
    source/c++/pyboost_ipc/api.inl.hpp
    source/c++/pyboost_ipc/internal.hpp
    DESTINATION include/pyboost_ipc
)
