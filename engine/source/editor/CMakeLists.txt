message("editor dir: ${EDITOR_DIR}")

set(TARGET_NAME Editor)

file(GLOB_RECURSE EDITOR_SOURCES ${EDITOR_DIR}/source *.cpp)

add_library( ${TARGET_NAME} STATIC ${EDITOR_SOURCES} )

set( EDITOR_INCLUDE_DIR ${EDITOR_DIR}/include )

target_link_libraries(${TARGET_NAME} PUBLIC Engine)

target_include_directories(${TARGET_NAME} PUBLIC Engine 
                                          PUBLIC ${EDITOR_INCLUDE_DIR})