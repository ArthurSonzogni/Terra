cmake_minimum_required(VERSION 2.6)
project(terra)
set(EXECUTABLE_OUTPUT_PATH bin)


#add bullet physics
include_directories(/usr/local/include/bullet)
link_directories(/usr/local/lib)

# executable configuration
file(GLOB_RECURSE source_file src/*)
add_executable(terra ${source_file})

# link configuration
target_link_libraries(terra BulletDynamics.so GLEW jpeg BulletCollision
	LinearMath sfml-network sfml-audio sfml-graphics sfml-window sfml-system GL
	GLU glut)
