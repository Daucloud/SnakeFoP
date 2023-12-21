cmake_minimum_required(VERSION 3.28.1)
project(SnakeFoP VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
include_directories(src/include)

add_executable(SnakeFoP 
src/code/funda.cpp 
src/code/play.cpp 
src/code/extension.cpp 
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
