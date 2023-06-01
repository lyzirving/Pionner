message("editor dir: ${EDITOR_DIR}")

set(TARGET_NAME Editor)

file(GLOB_RECURSE EDITOR_SOURCES ${EDITOR_DIR} *.cpp)

add_library( ${TARGET_NAME} STATIC ${EDITOR_SOURCES} )

target_link_libraries(${TARGET_NAME} PUBLIC Engine)

target_include_directories(${TARGET_NAME} PUBLIC Engine 
                                          PUBLIC ${EDITOR_DIR})