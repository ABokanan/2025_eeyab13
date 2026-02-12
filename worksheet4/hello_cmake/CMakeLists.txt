cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project (hello)
add_executable (hello hello.cpp)



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

# --- INSTALLATION SETUP ---
# Define where the compiled files should go inside the installer
install (TARGETS calc maths
    RUNTIME DESTINATION bin       # Executables (.exe) and DLLs go here
    LIBRARY DESTINATION lib       # Shared libraries (Linux .so) go here
    ARCHIVE DESTINATION lib/static # Static libraries (.lib) go here
)