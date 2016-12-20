# Options:
#
# [optional] - COMPILE_TESTS: Will compile the 'pyboost_ipc_tests' c++ module too. This module is necessary to run the tests of the project.
#

cmake_minimum_required(VERSION 3.0)

project(pyboost_ipc)

option(PY_DEBUG "Set if python being linked is a Py_DEBUG build" Off)

if (${PY_DEBUG})
    # see: http://www.boost.org/doc/libs/1_59_0/libs/python/doc/building.html#python-debugging-builds
    add_definitions(-DBOOST_DEBUG_PYTHON)
    set(Boost_USE_DEBUG_PYTHON True)
    set(CMAKE_DEBUG_POSTFIX _d)
    set(PYTHON_EXECUTABLE python_d)
endif()

find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)

execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(True))" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "Python directories being used:")
message(STATUS "  PYTHON_LIBRARY=${PYTHON_LIBRARY}")
message(STATUS "  PYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIRS}")

find_package(Boost 1.55 REQUIRED COMPONENTS python)

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

# Tests
option(COMPILE_TESTS "Compile test modules (necessary to run the python tests)" false)
if (${COMPILE_TESTS})
    # TARGET=pyboost_ipc_tests
    python_add_module(pyboost_ipc_tests source/c++/pyboost_ipc_tests/_pyboost_ipc_tests.cpp)
    target_link_libraries(pyboost_ipc_tests ${Boost_LIBRARIES})

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
