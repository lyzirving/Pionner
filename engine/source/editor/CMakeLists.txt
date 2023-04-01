message("editor dir: ${CMAKE_CURRENT_SOURCE_DIR}")

set(TARGET_NAME Editor)

file(GLOB_RECURSE EDITOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

add_library( ${TARGET_NAME} ${EDITOR_SOURCES} )

set( EDITOR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_link_libraries(${TARGET_NAME} PUBLIC Engine)

target_include_directories(${TARGET_NAME} PUBLIC Engine 
                                          PUBLIC ${EDITOR_INCLUDE_DIR})