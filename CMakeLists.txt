cmake_minimum_required(VERSION 3.18)

# Name of project
set(PROJ_NAME FinalProject)
project(${PROJ_NAME})

# Specify project files: header files and source files
set(HDRS
	file_utils.h
	game.h
	game_object.h
	player_game_object.h
	collectible_game_object.h
	fuel_collectible_object.h
	shield_collectible_object.h
	health_collectible_object.h
	enemy_game_object.h
	mothership_boss.h
	boomer_enemy.h
	dreadnought_enemy.h
	fighter_enemy.h
	boss_barrier.h
	celestial_body.h
	projectile.h
	pulse.h
	hierarchical_transformation.h
	shader.h
	geometry.h
	sprite.h
	timer.h
	particles.h
	particle_system.h
	drawing_game_object.h
	text_game_object.h
)
 
set(SRCS
	file_utils.cpp
	game.cpp
	game_object.cpp
	main.cpp
	player_game_object.cpp
	collectible_game_object.cpp
	fuel_collectible_object.cpp
	shield_collectible_object.cpp
	health_collectible_object.cpp
	enemy_game_object.cpp
	mothership_boss.cpp
	boomer_enemy.cpp
	dreadnought_enemy.cpp
	fighter_enemy.cpp
	boss_barrier.cpp
	celestial_body.cpp
	projectile.cpp
	pulse.cpp
	hierarchical_transformation.cpp
	shader.cpp
	sprite.cpp
	timer.cpp
	particles.cpp
	particle_system.cpp
	drawing_game_object.cpp
	text_game_object.cpp
	particle_vertex_shader.glsl
	particle_fragment_shader.glsl
	sprite_vertex_shader.glsl
	sprite_fragment_shader.glsl
	drawing_fragment_shader.glsl
	text_fragment_shader.glsl
)

# Add path name to configuration file
configure_file(path_config.h.in path_config.h)

# Add executable based on the source files
add_executable(${PROJ_NAME} ${HDRS} ${SRCS})

# Directories to include for header files, so that the compiler can find
# path_config.h
target_include_directories(${PROJ_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Require OpenGL library
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH $ENV{COMP2501_LIBRARY_PATH} CACHE PATH "Folder with GLEW, GLFW, GLM, and SOIL libraries")
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(${PROJ_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLFW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${SOIL_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

    # Set the default project in VS
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})
endif(WIN32)
