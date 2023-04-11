message("engine dir: ${CMAKE_CURRENT_SOURCE_DIR}")

set(ENGINE_DIR      ${PIONNER_ROOT_DIR}/engine)

set(RUNTIME_DIR     ${ENGINE_DIR}/source/runtime)
set(EDITOR_DIR      ${ENGINE_DIR}/source/editor)
set(THIRD_PARTY_DIR ${ENGINE_DIR}/3rdparty)
set(ASSETS_DIR      ${ENGINE_DIR}/assets)

add_subdirectory(3rdparty)
add_subdirectory(source/runtime)
add_subdirectory(source/editor)

set(TARGET_NAME Pionner)

add_executable (${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} PRIVATE Editor)
target_include_directories(${TARGET_NAME} PRIVATE Editor)

set_property(TARGET Pionner PROPERTY CXX_STANDARD 11)

add_custom_command(TARGET Pionner POST_BUILD                     # Adds a post-build event to Pionner
                   COMMAND ${CMAKE_COMMAND} -E copy_directory    # which executes "cmake - E copy_directory/copy_if_different..."
                   "${THIRD_PARTY_DIR}/libs/bin"                 # <--this is in-file
                   $<TARGET_FILE_DIR:Pionner>)

add_custom_command(TARGET Pionner POST_BUILD                  
                   COMMAND ${CMAKE_COMMAND} -E copy_directory    
                   "${ASSETS_DIR}"                           
                   $<TARGET_FILE_DIR:Pionner>/assets)

