cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)

project(cthulhu_interpreter)

include_directories(${CMAKE_SOURCE_DIR}/src/builtins/include)
include_directories(${CMAKE_SOURCE_DIR}/src/interpreter/include)
include_directories(${CMAKE_SOURCE_DIR}/src/parser/include)
include_directories(${CMAKE_SOURCE_DIR}/src/utils/include)

set(SOURCE_FILES ${SOURCE_FILES}
${CMAKE_SOURCE_DIR}/src/interpreter/source/interpreter.cpp)


add_executable(cthulhu src/main.cpp ${SOURCE_FILES})
