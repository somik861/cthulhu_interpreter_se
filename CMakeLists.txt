cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

project(cthulhu_se_interpreter)

macro(add_project_module module_path)
	include_directories("${module_path}/include")
	file(GLOB_RECURSE __SRC_FILES "${module_path}/source/*.cpp")
	set(SOURCE_FILES ${SOURCE_FILES} ${__SRC_FILES})
endmacro()

add_project_module("${CMAKE_SOURCE_DIR}/src/program")
add_project_module("${CMAKE_SOURCE_DIR}/src/misc")

add_executable(cthulhu_se src/main.cpp ${SOURCE_FILES})
