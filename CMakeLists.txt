cmake_minimum_required(VERSION 3.20)
set(projectName "FlipApp")
set(SOURCES 
    MyApp.cpp 
    Flip.cpp 
    FlipDataViewer.cpp
    FlipMain.cpp 
    FlipProgramLog.cpp
    FlipTemplateEditor.cpp
    cabjiFunctions.cpp
    include/StartupArgumentsParser.cpp
    main.exe.manifest
   )

project(${projectName} VERSION 0.2.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_GENERATOR "Unix Makefiles")

# msys2 root directory - you may need to set this to the correct directory for your system
set(MSYS2_ROOT "C:/msys64/ucrt64")
# wxWidgets specific configuration - set this as you require
set(wxWidgets_CONFIGURATION "mswu")
# !! triple check this, it's usually the problem - you may need to set this to the correct directory for your system
set(wx_LIB_BUILT_DIRNAME "gcc_x64_dll")

# ensure you have environment variable WXWIN set in your OS
set(wxWidgets_ROOT_DIR $ENV{WXWIN})
set(wxWidgets_LIB_DIR $ENV{WXWIN}/lib/${wx_LIB_BUILT_DIRNAME})
set(wxWidgets_INCLUDE_DIRS
      "${wxWidgets_ROOT_DIR}/include"
      "${wxWidgets_ROOT_DIR}/lib/${wx_LIB_BUILT_DIRNAME}/${wxWidgets_CONFIGURATION}"
   )

include_directories(
      $ENV{WXWIN}/include
      $ENV{WXWIN}/lib/gcc_x64_lib/mswud
      )

link_directories($ENV{WXWIN}/lib/gcc_x64_lib)

# Find wxWidgets for static linking
find_package(wxWidgets 3.2 REQUIRED COMPONENTS core base)

# Link against Poppler libraries
find_library(POPPLER_LIB poppler REQUIRED HINTS "${MSYS2_ROOT}/lib")
find_library(POPPLER_CPP_LIB poppler-cpp REQUIRED HINTS "${MSYS2_ROOT}/lib")

include(${wxWidgets_USE_FILE})
set(wxWidgets_USE_STATIC OFF)

# add sources to executable
add_executable(${projectName} WIN32 ${SOURCES})

target_include_directories(${projectName} PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${projectName} ${wxWidgets_LIBRARIES}  ${POPPLER_LIB} ${POPPLER_CPP_LIB})

# Handle project resources...
# Define the source and destination resource directories
set(RESOURCE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(RESOURCE_DEST_DIR "${CMAKE_BINARY_DIR}/resources")

# Create a custom target to copy the resources recursively
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE_SOURCE_DIR} ${RESOURCE_DEST_DIR}
    COMMENT "Copying resource files recursively to the build directory..."
)

# Create a custom command to clean the destination directory
add_custom_command(TARGET copy_resources PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${RESOURCE_DEST_DIR}
    COMMENT "Removing outdated resource files from the build directory..."
)

# Add a custom command to copy the resources recursively
add_custom_command(TARGET copy_resources POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE_SOURCE_DIR} ${RESOURCE_DEST_DIR}
    COMMENT "Copying resource files recursively to the build directory..."
)

    # Dynamic Linked Build for wxWidgets - Copy required DLLs to the build folder
    # Only uncomment the section below if you want to build dynamically linked executable of your project

    # !! - you probably need to alter this: DLL_SUFFIX - set the suffix of the DLLs in your lib directory (this is usually the wxWidgets lib/gcc_x64_dll directory)
    set (DLL_SUFFIX "_gcc_custom.dll")

    # loop to find and copy the required DLLs to the build directory
    foreach(dll ${wxWidgets_LIBRARIES})
        get_filename_component(dll_path ${dll} DIRECTORY)
        get_filename_component(dll_name ${dll} NAME_WE)
        if (dll_name MATCHES "^lib")
            message(STATUS "DLL Path: ${dll_path}, DLL Name: ${dll_name}")
            string(REGEX REPLACE "^lib" "" dll_name ${dll_name})
            set(dll_file "${dll_path}/${dll_name}${DLL_SUFFIX}")
            message(STATUS "Found DLL file: ${dll_file}")
            if(EXISTS ${dll_file})
            message(STATUS "Setting up copy command for DLL: ${dll_file}")
            add_custom_command(TARGET ${projectName} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${dll_file}
                $<TARGET_FILE_DIR:${projectName}>
                COMMENT "Copying required DLL: ${dll_file} to build/"
            )
            else()
                message(WARNING "DLL file not found: ${dll_file}")
            endif()
        endif()
    endforeach()

# Ensure the resources are copied before building the executable
add_dependencies(${projectName} copy_resources)
