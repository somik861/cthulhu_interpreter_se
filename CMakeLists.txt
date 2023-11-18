cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)

project(cthulhu_interpreter)

macro(add_project_module module_path)
	include_directories("${module_path}/include")
	file(GLOB __SRC_FILES "${module_path}/source/*.cpp")
	set(SOURCE_FILES ${SOURCE_FILES} ${__SRC_FILES})
endmacro()

add_project_module("${CMAKE_SOURCE_DIR}/src/builtins")
add_project_module("${CMAKE_SOURCE_DIR}/src/interpreter")
add_project_module("${CMAKE_SOURCE_DIR}/src/parser")
add_project_module("${CMAKE_SOURCE_DIR}/src/utils")

add_executable(cthulhu src/main.cpp ${SOURCE_FILES})
