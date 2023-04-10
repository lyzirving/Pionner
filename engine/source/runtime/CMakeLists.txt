message("runtime dir: ${RUNTIME_DIR}")

set(TARGET_NAME Engine)

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

file(GLOB_RECURSE ENGINE_SOURCES ${RUNTIME_DIR} *.cpp)

add_library( ${TARGET_NAME} STATIC ${ENGINE_SOURCES} )

set( ENGINE_INCLUDE_DIR ${RUNTIME_DIR} )

set( GLEW_LIB ${THIRD_PARTY_DIR}/glew/lib/x64/glew32s.lib)
set( GLEW_INCLUDE ${THIRD_PARTY_DIR}/glew/include)

add_library(assimp STATIC IMPORTED)
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/libs/assimp-vc143-mt.lib )

target_link_libraries(${TARGET_NAME} PUBLIC spdlog::spdlog
                                     PUBLIC ${OPENGL_LIBRARIES}
                                     PUBLIC glfw
                                     PUBLIC ${GLEW_LIB}
                                     PUBLIC imgui
                                     PUBLIC glm
                                     PUBLIC stb
                                     PUBLIC assimp)

target_include_directories( ${TARGET_NAME} PUBLIC ${ENGINE_INCLUDE_DIR}
                                           PUBLIC ${THIRD_PARTY_DIR}
                                           PUBLIC spdlog::spdlog
                                           PUBLIC glfw
                                           PUBLIC ${GLEW_INCLUDE}
                                           PUBLIC imgui
                                           PUBLIC glm 
                                           PUBLIC stb )