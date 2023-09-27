cmake_minimum_required(VERSION 3.10)

project(PhysicsEngine)

set(CMAKE_OSX_ARCHITECTURES ${ARCHS_STANDARD} CACHE INTERNAL "" FORCE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

message(STATUS "[${PROJECT_NAME}] Adding SDL3")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL)

# check the operating system set up
if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set(OPERATING_SYSTEM "Apple")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(OPERATING_SYSTEM "Win32")
endif()

message(STATUS "[${PROJECT_NAME}] adding imgui as subdirectory")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imgui)

message(STATUS "[${PROJECT_NAME}] linking imgui library")
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

message(STATUS "[${PROJECT_NAME}] adding forces as subdirectory")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/forces)
message(STATUS "[${PROJECT_NAME}] linking Forces library")
target_link_libraries(${PROJECT_NAME} PUBLIC Forces)


message(STATUS "[${PROJECT_NAME}] adding objects as subdirectory")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/objects)
message(STATUS "[${PROJECT_NAME}] linking Objects library")
target_link_libraries(${PROJECT_NAME} PUBLIC Objects)

