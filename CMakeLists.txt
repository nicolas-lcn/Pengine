# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Pengine)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# pengine
add_executable(pengine
        pengine/main.cpp
        pengine/GLTexture.cpp
        pengine/MeshObject.cpp
        pengine/Plane.cpp
        pengine/Sphere.cpp
        pengine/SceneObject.cpp
        pengine/Texture.cpp
        pengine/RigidBody.cpp
        pengine/BoxCollider.cpp
        pengine/CameraObject.cpp
        pengine/ShaderController.cpp
        pengine/Entity.cpp
        pengine/MenusRenderer.cpp
	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp
	common/vboindexer.cpp
	common/vboindexer.hpp
	common/geometry_utils.cpp
	common/geometry_utils.h
	common/quaternion_utils.cpp
	common/quaternion_utils.hpp
	common/tangentspace.hpp
	common/tangentspace.cpp
	common/text2D.hpp
	common/text2D.cpp
	
        pengine/shaders/vertex_shader.glsl
        pengine/shaders/fragment_shader.glsl
        pengine/shaders/MenuVertexShader.glsl
        pengine/shaders/MenuFragmentShader.glsl
)
target_link_libraries(pengine
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(pengine PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pengine/")
create_target_launcher(pengine WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pengine/")




SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET pengine POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/pengine${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/pengine/"
)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

