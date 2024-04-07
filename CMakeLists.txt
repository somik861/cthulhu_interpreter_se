cmake_minimum_required(VERSION 3.20)

project(cthulhu_new)
set(CMAKE_CXX_STANDARD 23)

# project modules
macro(add_project_module module_path)
	include_directories("${module_path}/include")
	file(GLOB_RECURSE __SRC_FILES "${module_path}/source/*.cpp")
	set(SOURCE_FILES ${SOURCE_FILES} ${__SRC_FILES})
endmacro()

add_project_module("${CMAKE_SOURCE_DIR}/src/builtin_structs")
add_project_module("${CMAKE_SOURCE_DIR}/src/common")
add_project_module("${CMAKE_SOURCE_DIR}/src/domains")
add_project_module("${CMAKE_SOURCE_DIR}/src/program")
add_project_module("${CMAKE_SOURCE_DIR}/src/interpreter")


# 3rd party libs

# Boost
find_package(Boost CONFIG REQUIRED)
set(LIBS ${LIBS} boost::boost)


add_executable(cthulhu src/main.cpp ${SOURCE_FILES})
target_link_libraries(cthulhu ${LIBS})

add_executable(tests tests/main.cpp  tests/Dict.cpp ${SOURCE_FILES})
target_link_libraries(tests ${LIBS})