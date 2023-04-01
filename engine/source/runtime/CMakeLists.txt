message("runtime dir: ${CMAKE_CURRENT_SOURCE_DIR}")

set(TARGET_NAME Engine)

file(GLOB_RECURSE ENGINE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

add_library( ${TARGET_NAME} ${ENGINE_SOURCES} )

set( ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries(${TARGET_NAME} PUBLIC spdlog::spdlog)

target_include_directories( ${TARGET_NAME} PUBLIC ${ENGINE_INCLUDE_DIR} 
                                           PUBLIC spdlog::spdlog)