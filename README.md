Python bindings for the  C++ library [Boost.Interprocess](http://www.boost.org/doc/libs/release/doc/html/interprocess.html).

[Boost.Python](http://www.boost.org/doc/libs/release/libs/python/doc/index.html) is the library used to create the bindings.

# Goal

To provide an almost 1-1 of the Boost.Interprocess library. It is not meant to expand on the features of Boost.Interprocess.

The ideal usage scenario is when you have a C++ code and you want to expose its data through IPC (using Boost.Interprocess) and receive that data in python.

# Building

Out-of-source builds will be assumed, if you don't know what that is, read about it here: http://www.cmake.org/Wiki/CMake_FAQ#Out-of-source_build_trees

```
cmake ..
cmake --build .
```

# Installation

The cmake scripts will use what the cmake `find_package(PythonLibs)` returns. In case this can't be found, it will attempt to use the current python (by calling `python` in the command-line) in order to find where the `site-packages` directory is.

- On Linux: `make install`
- On Windows using Visual Studio: build the `INSTALL` project inside Visual Studio.

# Running tests

Tests are written using [pytest](http://pytest.org/latest/). CTest can be used to run them.

`ctest .`

# Using the library

Bindings for common types are provided. This can be a rather limited scenario though, since Boost.Interprocess is a C++ library it relies heavily on the types being "communicated".

To benefit from these types an api was defined. It can be seen at the file `<pyboost_ipc/api.hpp>`.

The main method is:

```
template <typename Type>
void register_ipc_type();
```

By calling this method on your own bindings you will be able to use the `managed_shared_memory.find()` passing the type of the registered type.

In order to make it work you also have to link with the `pyboost_ipc` library.

# Compilation troubleshooting

## Boost not found

Pass the `BOOST_ROOT` variable to CMake:

`cmake .. -DBOOST_ROOT=/path/to/boost`

## Python libs nor interpreter not found

You must pass these variables to cmake:

```
PYTHON_LIBRARY             - path to the python library
PYTHON_INCLUDE_DIR         - path to where Python.h is found
```

Like this:

`cmake .. -DPYTHON_LIBRARY=/path/to/python.lib -DPYTHON_INCLUDE_DIR=/path/to/python.h`

## Visual studio 2013 compilation error on Boost 1.55

If you see this error:
```
left of '.select_on_container_copy_construction' must have class/struct/union
```

This means that the [Boost.Container](http://www.boost.org/doc/libs/release/doc/html/container.html) library has problems with this version of boost. See this [boost ticket](https://svn.boost.org/trac/boost/ticket/9332).

You can either:

- Upgrade boost
- Apply the patch proposed in the ticket