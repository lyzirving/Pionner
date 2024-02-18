set(imgui_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

message("imgui dir: ${imgui_SOURCE_DIR_}")

file(GLOB imgui_sources CONFIGURE_DEPENDS  "${imgui_SOURCE_DIR_}/*.cpp")
file(GLOB imgui_impl CONFIGURE_DEPENDS  
"${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.cpp" 
"${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.h"
"${imgui_SOURCE_DIR_}/backends/imgui_impl_opengl3.cpp" 
"${imgui_SOURCE_DIR_}/backends/imgui_impl_opengl3.h"
"${imgui_SOURCE_DIR_}/backends/imgui_impl_opengl3_loader.h")
add_library(imgui STATIC ${imgui_sources} ${imgui_impl})

find_package(OpenGL REQUIRED)
message(STATUS "OpenGL_FOUND:= ${OpenGL_FOUND}")

if(OpenGL_FOUND)
    message('OpenGL_FOUND-is-true')
else(OpenGL_FOUND)
    message('OpenGL_FOUND-is-false')
endif(OpenGL_FOUND)

message(STATUS "OPENGL_INCLUDE_DIR:= ${OPENGL_INCLUDE_DIR}")
message(STATUS "OPENGL_LIBRARY:= ${OPENGL_LIBRARY}")
message(STATUS "OPENGL_LIBRARIES:= ${OPENGL_LIBRARIES}")

target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${imgui_SOURCE_DIR_}>)
target_link_libraries(imgui PUBLIC glfw ${OPENGL_LIBRARIES})