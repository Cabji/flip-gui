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

set (CMAKE_CXX_FLAGS "-mwindows")

find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

add_executable(FlipApp 
    MyApp.cpp 
    Flip.cpp 
    FlipMain.cpp 
    FlipProgramLog.cpp
    app.manifest  
)

target_include_directories(FlipApp PRIVATE ${wxWidgets_INCLUDE_DIRS})

target_link_libraries(FlipApp PRIVATE ${wxWidgets_LIBRARIES})