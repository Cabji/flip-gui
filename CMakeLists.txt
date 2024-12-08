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

# wxWidgets specific configuration
set(wxWidgets_CONFIGURATION "mswud")
set(wxWidgets_ROOT_DIR $ENV{WXWIN})
set(wxWidgets_LIB_DIR $ENV{WXWIN}/lib/gcc_x64_lib)
set(wxWidgets_INCLUDE_DIRS
      "${wxWidgets_ROOT_DIR}/include"
      "${wxWidgets_ROOT_DIR}/lib/gcc_x64_lib/mswud"
   )

include_directories(
      $ENV{WXWIN}/include
      $ENV{WXWIN}/lib/gcc_x64_lib/mswud
      )

link_directories($ENV{WXWIN}/lib/gcc_x64_lib)

# Find wxWidgets for static linking
find_package(wxWidgets 3.2 REQUIRED COMPONENTS gl core base)

# Link against Poppler libraries
find_library(POPPLER_LIB poppler REQUIRED HINTS "C:/msys64/ucrt64/lib")
find_library(POPPLER_CPP_LIB poppler-cpp REQUIRED HINTS "C:/msys64/ucrt64/lib")

include(${wxWidgets_USE_FILE})
set(wxWidgets_USE_STATIC ON)

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

# Ensure the resources are copied before building the executable
add_dependencies(${projectName} copy_resources)
