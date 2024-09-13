cmake_minimum_required(VERSION 3.20)
project(FlipApp)

set(wxWidgets_ROOT_DIR "c:/cpp/cppLibraries/wxWidgets_static")
set(wxWidgets_LIB_DIR "${wxWidgets_ROOT_DIR}/lib/gcc_x64_lib")
set(wxWidgets_CONFIGURATION "mswu")
set(wxWidgets_INCLUDE_DIRS 
    "${wxWidgets_ROOT_DIR}/include/wx/msw" 
    "${wxWidgets_LIB_DIR}mswu/" 
    "${CMAKE_SOURCE_DIR}"
)

set(CMAKE_CXX_FLAGS "-mwindows")

find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

add_executable(FlipApp 
    MyApp.cpp 
    Flip.cpp 
    FlipMain.cpp 
    FlipProgramLog.cpp
    FlipTemplateEditor.cpp
    app.manifest  
)

target_include_directories(FlipApp PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(FlipApp PRIVATE ${wxWidgets_LIBRARIES} Dwmapi)

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

# Assuming you are using MSYS2/MinGW or Visual Studio:
if (WIN32)
    add_custom_command(TARGET FlipApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E
            env "PATH=$ENV{PATH}"
            mt.exe -manifest \"${CMAKE_SOURCE_DIR}/app.manifest\" -outputresource:\"$<TARGET_FILE:FlipApp>\";
        COMMENT "Embedding manifest..."
    )
endif()

# Ensure the resources are copied before building the executable
add_dependencies(FlipApp copy_resources)
