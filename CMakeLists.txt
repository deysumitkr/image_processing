cmake_minimum_required(VERSION 3.10)
project(MyCppProject C CXX)

set(CMAKE_C_STANDARD 11)

# Add OpenCV install prefix to CMAKE_PREFIX_PATH
if(NOT DEFINED OpenCV_DIR)
    message(WARNING "OpenCV_DIR is not defined. Some executables depending on OpenCV will be skipped.")
else()
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${OpenCV_DIR})
endif()

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# ----------------------------------------------------
# Add opencv executable
add_executable(cv_hw src/main.cpp)

# Link OpenCV libraries to the executable
target_link_libraries(cv_hw ${OpenCV_LIBS})

# Install target for the binary
install(TARGETS cv_hw DESTINATION bin)

# ----------------------------------------------------

add_executable(video src/video.cpp)

# Link OpenCV libraries to the executable
target_link_libraries(video ${OpenCV_LIBS})

# Install target for the binary
install(TARGETS video DESTINATION bin)

# ----------------------------------------------------

add_executable(live_video src/liveVideo.cpp)

# Link OpenCV libraries to the executable
target_link_libraries(live_video ${OpenCV_LIBS})

# Install target for the binary
install(TARGETS live_video DESTINATION bin)
