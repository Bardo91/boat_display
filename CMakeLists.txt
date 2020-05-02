#---------------------------------------------------------------------------------------------------------------------
#  Rosex Hidraulica Sl
#---------------------------------------------------------------------------------------------------------------------
# 
#  Copyright 2020 Rosex Hidraulica Sl  - All Rights Reserved
# 
#  Unauthorized copying of this file, via any medium is strictly prohibited Proprietary and confidential.
# 
#  All information contained herein is, and remains the property of Vertical Engineering Solutions.  The 
#  intellectual and technical concepts contained herein are proprietary to Vertical Engineering Solutions 
#  and its suppliers and may be covered by UE and Foreign Patents, patents in process, and are protected 
#  by trade secret or copyright law. Dissemination of this information or reproduction of this material is 
#  strictly forbidden unless prior written permission is obtained from Adobe Systems Incorporated.
#
#---------------------------------------------------------------------------------------------------------------------
#
#  Maintainer: pabramsor@gmail.com
#
#---------------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.12.0)

project(boat_display VERSION 0.1 LANGUAGES CXX)


## Global Configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


## Project configuration
file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE HDR_FILES include/*.h include/*.inl)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ./include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
# target_link_directories(${PROJECT_NAME} PRIVATE /usr/local/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE pthread)

## Project dependenciess
find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC serial)


find_package(yaml-cpp REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${YAML_CPP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${YAML_CPP_LIBRARIES} yaml-cpp)


install(DIRECTORY resources DESTINATION $ENV{HOME}/.boat_display FILES_MATCHING PATTERN "*")
install(DIRECTORY config DESTINATION $ENV{HOME}/.boat_display FILES_MATCHING PATTERN "*")
