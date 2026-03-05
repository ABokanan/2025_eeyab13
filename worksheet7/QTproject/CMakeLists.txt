cmake_minimum_required(VERSION 3.19)
project(QTproject LANGUAGES CXX)

# --- MISSING FIX 1: Prevent "Command line too long" error ---
set( CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files." )

# --- MISSING FIX 2: Tell CMake where OpenVR is BEFORE it finds VTK ---
# CHANGE THESE PATHS TO MATCH YOUR OPENVR FOLDER!
set(OpenVR_INCLUDE_DIR "C:/OpenVR/headers" CACHE PATH "")
set(OpenVR_LIBRARY "C:/OpenVR/lib/win64/openvr_api.lib" CACHE FILEPATH "")
# ----------------------------------------------------------------------

#addition 1
find_package (VTK REQUIRED)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(QTproject
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    icons.qrc
    optiondialog.h
    optiondialog.cpp
    optiondialog.ui
)

target_link_libraries(QTproject
    PRIVATE
        Qt::Core
        Qt::Widgets
        ${VTK_LIBRARIES} #addition 2
)

# --- MISSING FIX 3: Initialize VTK Modules (Required for 3D rendering) ---
vtk_module_autoinit( TARGETS QTproject MODULES ${VTK_LIBRARIES} )

include(GNUInstallDirs)

install(TARGETS QTproject
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QTproject
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# addition 3
# Copy across Open VR config files
# These exist in VTK bin dir but for some reason VR classes expect them to be in local dir
add_custom_target(  VRBindings )
add_custom_command( TARGET VRBindings PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E
        copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/ )
