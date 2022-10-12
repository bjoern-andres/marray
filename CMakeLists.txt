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

# doxygen
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    message(STATUS "Doxygen found.")
else()
    message("doxygen not found.")
endif()

# hdf5
find_package(HDF5)
if(HDF5_FOUND)
    include_directories(${HDF5_INCLUDE_DIR})
endif()

# png
find_package(PNG)
if(PNG_FOUND)
    include_directories(${PNG_INCLUDE_DIR})
endif()

# targets
add_executable(tutorial-marray src/tutorial/tutorial.cxx ${headers})

add_executable(test-marray src/unittest/marray.cxx ${headers})
add_test(test-marray test-marray)

add_executable(test-marray-bmp src/unittest/marray-bmp.cxx ${headers})
add_test(test-marray-bmp test-marray-bmp)

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

if(PNG_FOUND)
    add_executable(test-marray-png src/unittest/marray-png.cxx ${headers})
    target_link_libraries(test-marray-png ${PNG_LIBRARIES})
    add_test(test-marray-png test-marray-png)
endif()

if(DOXYGEN_FOUND)
    configure_file("${marray_SOURCE_DIR}/doxygen/Doxyfile.in" "${marray_BINARY_DIR}/Doxyfile" @ONLY IMMEDIATE)
    add_custom_target(doc-marray ALL COMMAND ${DOXYGEN} "${marray_BINARY_DIR}/Doxyfile")
endif()

