cmake_minimum_required(VERSION 2.6)
enable_testing()
project(marray)
include_directories(include include/andres)
file(GLOB headers include/andres/*)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall)
endif()

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_HDF5USEDLL_)
endif()

# ------------------------------------------------------------------------------
# doxygen
# ------------------------------------------------------------------------------
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    message(STATUS "Doxygen found.")
else()
    message("doxygen not found.")
endif()

# ------------------------------------------------------------------------------
# C++11
# ------------------------------------------------------------------------------
set(WITH_CPP11 CACHE BOOL "Compile with C++11 features.")
if(WITH_CPP11)
    if(CMAKE_COMPILER_IS_GNUCXX)
        message(STATUS "C++11 support enabled.")
        add_definitions(-std=gnu++0x)
        add_definitions(-fno-deduce-init-list)
        add_definitions(-DHAVE_CPP11_INITIALIZER_LISTS)
        add_definitions(-DHAVE_CPP11_VARIADIC_TEMPLATES)
        # add_definitions(-DHAVE_CPP11_TEMPLATE_ALIASES) # not supported in GNU G++ 4.4
    else()
        message("C++11 support disabled. Add flags for your compiler in CMakeLists.txt to compile with C++11 features.")
    endif()
else()
    message(STATUS "C++11 support disabled.")
endif()

# ------------------------------------------------------------------------------
# hdf5
# ------------------------------------------------------------------------------
find_package(HDF5)

# ------------------------------------------------------------------------------
# targets
# ------------------------------------------------------------------------------
add_executable(tutorial-marray src/tutorial/tutorial.cxx ${headers})

add_executable(test-marray src/unittest/marray.cxx ${headers})
add_test(test-marray test-marray)

if(HDF5_FOUND)
    add_executable(test-hdf5 src/unittest/hdf5.cxx ${headers})
    target_link_libraries(test-hdf5 ${HDF5_LIBRARIES})
    add_test(test-hdf5 test-hdf5)

    add_executable(test-marray-hdf5 src/unittest/marray-hdf5.cxx ${headers})
    target_link_libraries(test-marray-hdf5 ${HDF5_LIBRARIES})
    add_test(test-marray-hdf5 test-marray-hdf5)

    add_executable(tutorial-marray-hdf5 src/tutorial/tutorial-hdf5.cxx ${headers})
    target_link_libraries(tutorial-marray-hdf5 ${HDF5_LIBRARIES})
endif()

if(DOXYGEN_FOUND)
    configure_file("${marray_SOURCE_DIR}/doxygen/Doxyfile.in" "${marray_BINARY_DIR}/Doxyfile" @ONLY IMMEDIATE)
    add_custom_target(doc-marray ALL COMMAND ${DOXYGEN} "${marray_BINARY_DIR}/Doxyfile")
endif()
