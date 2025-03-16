cmake_minimum_required(VERSION 3.10)
project(MyCppProject C CXX)

set(CMAKE_C_STANDARD 11)

add_executable(my_cpp_project src/main.c)

# Install target for the binary
install(TARGETS my_cpp_project DESTINATION bin)
