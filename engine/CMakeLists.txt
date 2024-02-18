message("engine dir: ${CMAKE_CURRENT_SOURCE_DIR}")

set(ENGINE_DIR      ${PIONNER_ROOT_DIR}/engine)
set(SOURCE_DIR      ${ENGINE_DIR}/src)
set(THIRD_PARTY_DIR ${ENGINE_DIR}/3rdparty)
set(ASSETS_DIR      ${ENGINE_DIR}/assets)
set(SHADER_DIR      ${ENGINE_DIR}/shader)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    message("build type: ${CMAKE_BUILD_TYPE}")
    set(PIO_PROJECT_BUILD_TYPE debug)
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    message("build type: ${CMAKE_BUILD_TYPE}")
    set(PIO_PROJECT_BUILD_TYPE release)
else()
    message("CMAKE_BUILD_TYPE is empty: ${CMAKE_BUILD_TYPE}")
    set(PIO_PROJECT_BUILD_TYPE debug)
endif()

add_subdirectory(3rdparty)
add_subdirectory(src)

set(TARGET_NAME Pionner)

add_executable (${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} PRIVATE Engine)
target_include_directories(${TARGET_NAME} PRIVATE Engine)

set_property(TARGET Pionner PROPERTY CXX_STANDARD 11)

add_custom_command(TARGET Pionner POST_BUILD                              # Adds a post-build event to Pionner
                   COMMAND ${CMAKE_COMMAND} -E copy_directory             # which executes "cmake - E copy_directory/copy_if_different..."
                   "${THIRD_PARTY_DIR}/libs/physx/bin/${PIO_PROJECT_BUILD_TYPE}"   # <--this is in-file
                   $<TARGET_FILE_DIR:Pionner>)

add_custom_command(TARGET Pionner POST_BUILD                       
                   COMMAND ${CMAKE_COMMAND} -E copy_directory 
                   "${THIRD_PARTY_DIR}/libs/assimp/bin"         
                   $<TARGET_FILE_DIR:Pionner>)

add_custom_command(TARGET Pionner POST_BUILD                  
                   COMMAND ${CMAKE_COMMAND} -E copy_directory    
                   "${ASSETS_DIR}"                           
                   $<TARGET_FILE_DIR:Pionner>/assets)

add_custom_command(TARGET Pionner POST_BUILD                  
                   COMMAND ${CMAKE_COMMAND} -E copy_directory    
                   "${SHADER_DIR}"                           
                   $<TARGET_FILE_DIR:Pionner>/shader)

