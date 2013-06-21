cmake_minimum_required(VERSION 2.6)
project(terra)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

#add SFML
include_directories(sfml2_include)
link_directories(sfml2_lib)

# executable configuration
file(GLOB_RECURSE source_file src/*)
add_executable(terra ${source_file})

# link configuration
target_link_libraries(terra sfml-audio sfml-graphics sfml-window sfml-system GL
	glut)
