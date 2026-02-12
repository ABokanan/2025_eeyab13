#calc_cmake/CMakeLists.txt

cmake_minimum_required (VERSION 3.12 FATAL_ERROR)
project ( calc )

# This project contains a library
add_library ( maths adder.cpp )
# Note: you could force CMake to always build a static/shared library:
#add_library( maths STATIC adder.cpp )
#add_library( maths SHARED adder.cpp )

if ( NOT BUILD_SHARED_LIBS )
# if static libs are compiled we need
# to somehow communicate that to the 
# source code. The definition will be
# used to determine how MATHSLIB_API
# is defined in adder.h
add_definitions ( -Dmaths_STATIC )
endif ()

# It also contains an executable
add_executable( calc calc.cpp )

# The executable uses the library
target_link_libraries (calc maths)

# /calc_cmake/CMakeLists.txt
# --- PACKAGING SETUP ---
# If on Windows, use NSIS to create an .exe installer.
# If on Linux/Mac, use ZIP to create a compressed folder.
if (WIN32)
    set (CPACK_GENERATOR "NSIS")
else ()
    set (CPACK_GENERATOR "ZIP")
endif ()

# Initialize the CPack tool
include (CPack)

# add_library (maths adder.cpp)
# add_executable (calc calc.cpp)
#target_link_libraries (calc maths)

# --- INSTALLATION SETUP ---
# Define where the compiled files should go inside the installer
install (TARGETS calc maths
    RUNTIME DESTINATION bin       # Executables (.exe) and DLLs go here
    LIBRARY DESTINATION lib       # Shared libraries (Linux .so) go here
    ARCHIVE DESTINATION lib/static # Static libraries (.lib) go here
)