message("3rdparty dir: ${THIRD_PARTY_DIR}")

if(NOT TARGET spdlog)
    option(SPDLOG_BUILD_EXAMPLE "" OFF)
    option(SPDLOG_INSTALL "" OFF)
    add_subdirectory(spdlog)
    set_target_properties(spdlog PROPERTIES FOLDER ${THIRD_PARTY_DIR}/spdlog)
endif()

if(NOT TARGET imgui)
    include(imgui.cmake)
    set_target_properties(imgui PROPERTIES FOLDER ${THIRD_PARTY_DIR}/imgui)
endif()

if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${THIRD_PARTY_DIR}/glfw)
    set_target_properties(update_mappings PROPERTIES FOLDER ${THIRD_PARTY_DIR}/glfw)
endif()

if(NOT TARGET glm)
    option(BUILD_STATIC_LIBS "" ON)
    add_subdirectory(glm)
    option(BUILD_STATIC_LIBS "" OFF)
endif()

if(NOT TARGET stb)
    include(stb.cmake)
    set_target_properties(stb PROPERTIES FOLDER ${THIRD_PARTY_DIR}/stb)
endif()

if(NOT TARGET Decs)
    add_library(Decs INTERFACE)

    target_include_directories(
        Decs INTERFACE ${THIRD_PARTY_DIR}/decs)
endif()
