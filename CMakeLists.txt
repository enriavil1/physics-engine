cmake_minimum_required(VERSION 3.10)

project(PhysicsEngine)

# Set C++ standard and features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CURRENT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
set(CMAKE_CURRENT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/lib)

set(USE_SDL ON)

# Define build options (e.g., build with SDL, ImGui)
option(BUILD_SDL "Build with SDL support" ON)
option(BUILD_IMGUI "Build with ImGui support" ON)
option(BUILD_OPENGL "Build with OpenGL support" ON)
option(BUILD_THREADS "Build with Multi thread support" ON)

# Check the operating system setup
if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set(OPERATING_SYSTEM "Darwin")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(OPERATING_SYSTEM "Win32")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(OPERATING_SYSTEM "Unix")
endif()

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_LIB_DIR}/**/*.cpp")
add_library(LibModule ${SOURCE_FILES})

if(BUILD_OPENGL)
  message(STATUS "[${PROJECT_NAME}] Adding OpenGL library")
  find_package(OpenGL REQUIRED)
  target_link_libraries(LibModule PUBLIC OpenGL::GL)
endif()

# Add and link dependencies based on build options
if(BUILD_IMGUI)
  message(STATUS "[${PROJECT_NAME}] Adding imgui as subdirectory")
  add_subdirectory(${CMAKE_CURRENT_INCLUDE_DIR}/imgui)
  target_link_libraries(LibModule PUBLIC imgui)
endif()

if(BUILD_SDL)
  message(STATUS "[${PROJECT_NAME}] Adding SDL3")
  add_subdirectory(${CMAKE_CURRENT_INCLUDE_DIR}/SDL)
  target_link_libraries(LibModule PUBLIC SDL3::SDL3)
endif()

add_executable(${PROJECT_NAME} main.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PROJECT_NAME} PUBLIC LibModule)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_INCLUDE_DIR}>)
