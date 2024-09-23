message("runtime dir: ${SOURCE_DIR}")

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

add_definitions(-DPIO_WINDOWS)

file(GLOB_RECURSE ENGINE_SOURCES ${SOURCE_DIR} *.cpp)

add_library( ${TARGET_NAME} STATIC ${ENGINE_SOURCES} )

set( ENGINE_INCLUDE_DIR ${SOURCE_DIR} )

set( GLEW_LIB ${THIRD_PARTY_DIR}/glew/lib/x64/glew32s.lib)
set( GLEW_INCLUDE ${THIRD_PARTY_DIR}/glew/include)

set( PHYSX_INCLUDE ${THIRD_PARTY_DIR}/physx/include)
set( PHYSX_FAST_XML_INCLUDE ${THIRD_PARTY_DIR}/physx/fastxml/include)
set( PHYSX_FOUNDATION_INCLUDE ${THIRD_PARTY_DIR}/physx/foundation/include)
set( PHYSX_SHARED_INCLUDE ${THIRD_PARTY_DIR}/physx/PxShared/include)

add_library(assimp STATIC IMPORTED)
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/libs/assimp/assimp-vc143-mt.lib )

add_library(physx STATIC IMPORTED)
set_target_properties(physx PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/libs/physx/${PIO_PROJECT_BUILD_TYPE}/PhysX_64.lib )

add_library(physx_character_kinematic STATIC IMPORTED)
set_target_properties(physx_character_kinematic PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/libs/physx/${PIO_PROJECT_BUILD_TYPE}/PhysXCharacterKinematic_static_64.lib )

add_library(physx_common STATIC IMPORTED)
set_target_properties(physx_common PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/libs/physx/${PIO_PROJECT_BUILD_TYPE}/PhysXCommon_64.lib )

add_library(physx_cooking STATIC IMPORTED)
set_target_properties(physx_cooking PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/libs/physx/${PIO_PROJECT_BUILD_TYPE}/PhysXCooking_64.lib )

add_library(physx_extensions STATIC IMPORTED)
set_target_properties(physx_extensions PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/libs/physx/${PIO_PROJECT_BUILD_TYPE}/PhysXExtensions_static_64.lib )

add_library(physx_foundation STATIC IMPORTED)
set_target_properties(physx_foundation PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/libs/physx/${PIO_PROJECT_BUILD_TYPE}/PhysXFoundation_64.lib )

add_library(physx_pvdsdk STATIC IMPORTED)
set_target_properties(physx_pvdsdk PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/libs/physx/${PIO_PROJECT_BUILD_TYPE}/PhysXPvdSDK_static_64.lib )

add_library(physx_task STATIC IMPORTED)
set_target_properties(physx_task PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/libs/physx/${PIO_PROJECT_BUILD_TYPE}/PhysXTask_static_64.lib )

add_library(physx_vehicle STATIC IMPORTED)
set_target_properties(physx_vehicle PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/libs/physx/${PIO_PROJECT_BUILD_TYPE}/PhysXVehicle_static_64.lib )

target_link_libraries(${TARGET_NAME} PUBLIC spdlog::spdlog
                                     PUBLIC ${OPENGL_LIBRARIES}
                                     PUBLIC glfw
                                     PUBLIC ${GLEW_LIB}
                                     PUBLIC imgui
                                     PUBLIC glm
                                     PUBLIC stb
                                     PUBLIC assimp
                                     PUBLIC entt
                                     PUBLIC MAGIC_ENUM
                                     PUBLIC physx
                                     PUBLIC physx_character_kinematic
                                     PUBLIC physx_common
                                     PUBLIC physx_cooking
                                     PUBLIC physx_extensions
                                     PUBLIC physx_foundation
                                     PUBLIC physx_pvdsdk
                                     PUBLIC physx_task
                                     PUBLIC physx_vehicle)

target_include_directories( ${TARGET_NAME} PUBLIC ${ENGINE_INCLUDE_DIR}
                                           PUBLIC ${THIRD_PARTY_DIR}
                                           PUBLIC spdlog::spdlog
                                           PUBLIC glfw
                                           PUBLIC ${GLEW_INCLUDE}
                                           PUBLIC imgui
                                           PUBLIC glm 
                                           PUBLIC stb
                                           PUBLIC entt
                                           PUBLIC MAGIC_ENUM
                                           PUBLIC ${PHYSX_INCLUDE}
                                           PUBLIC ${PHYSX_FAST_XML_INCLUDE}
                                           PUBLIC ${PHYSX_FOUNDATION_INCLUDE}
                                           PUBLIC ${PHYSX_SHARED_INCLUDE})