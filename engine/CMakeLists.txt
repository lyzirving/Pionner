message("engine dir: ${CMAKE_CURRENT_SOURCE_DIR}")

set(ENGINE_DIR      "${PIONNER_ROOT_DIR}/engine")

set(RUNTIME_DIR     "${ENGINE_DIR}/source/runtime")
set(THIRD_PARTY_DIR "${ENGINE_DIR}/3rdparty")

add_subdirectory(source/runtime)
add_subdirectory(source/editor)

set(TARGET_NAME Pionner)

add_executable (${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} PRIVATE Engine)
target_include_directories(${TARGET_NAME} PRIVATE Engine)

target_link_libraries(${TARGET_NAME} PRIVATE Editor)
target_include_directories(${TARGET_NAME} PRIVATE Editor)

set_property(TARGET Pionner PROPERTY CXX_STANDARD 11)

