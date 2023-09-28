cmake_minimum_required(VERSION 3.10)

project(PhysicsEngine)

# Set C++ standard and features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(${PROJECT_NAME} main.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# Define build options (e.g., build with SDL, ImGui, Forces, and Objects)
option(BUILD_SDL "Build with SDL support" ON)
option(BUILD_IMGUI "Build with ImGui support" ON)
option(BUILD_FORCES "Build with Forces library" ON)
option(BUILD_OBJECTS "Build with Objects library" ON)

# Check the operating system setup
if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set(OPERATING_SYSTEM "Apple")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(OPERATING_SYSTEM "Win32")
endif()

# Add and link dependencies based on build options
if(BUILD_SDL)
  message(STATUS "[${PROJECT_NAME}] Adding SDL3")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL)
endif()

if(BUILD_IMGUI)
  message(STATUS "[${PROJECT_NAME}] Adding imgui as subdirectory")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imgui)
  target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
endif()

if(BUILD_FORCES)
  message(STATUS "[${PROJECT_NAME}] Adding forces as subdirectory")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/forces)
  target_link_libraries(${PROJECT_NAME} PUBLIC Forces)
endif()

if(BUILD_OBJECTS)
  message(STATUS "[${PROJECT_NAME}] Adding objects as subdirectory")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/objects)
  target_link_libraries(${PROJECT_NAME} PUBLIC Objects)
endif()
