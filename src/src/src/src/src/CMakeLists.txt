cmake_minimum_required(VERSION 3.15)
project(VISION CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/overlay.cpp
    src/menu.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_win32.cpp
    imgui/backends/imgui_impl_dx11.cpp
)

add_executable(VISION WIN32 ${SOURCES})
target_include_directories(VISION PRIVATE src imgui imgui/backends)
target_link_libraries(VISION PRIVATE d3d11 dxgi d3dcompiler dwmapi)
