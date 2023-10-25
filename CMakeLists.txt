cmake_minimum_required(VERSION 3.10)

project(PhysicsEngine)

# Set C++ standard and features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(${PROJECT_NAME} main.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# Define build options (e.g., build with SDL, ImGui)
option(BUILD_SDL "Build with SDL support" ON)
option(BUILD_IMGUI "Build with ImGui support" ON)
option(BUILD_OPENGL "Build with OpenGL support" ON)
option(BUILD_THREADS "Build with Multi thread support" ON)

option(AMOUNT_OF_THREADS "The amount of threads used by the thread pool" 10)

# Check the operating system setup
if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set(OPERATING_SYSTEM "Apple")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(OPERATING_SYSTEM "Win32")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(OPERATING_SYSTEM "Unix")
endif()

if(BUILD_OPENGL)
  message(STATUS "[${PROJECT_NAME}] Adding OpenGL library")
  find_package(OpenGL REQUIRED)
  target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)
endif()

# Add and link dependencies based on build options
if(BUILD_SDL)
  message(STATUS "[${PROJECT_NAME}] Adding SDL3")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL)
  target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)
endif()

if(BUILD_THREADS)
  message(STATUS "[${PROJECT_NAME}] Adding ThreadPool")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/thread_pool)
  target_compile_definitions(
    ${PROJECT_NAME} PUBLIC USE_MULTI_THREADS=1
                           AMOUNT_OF_THREADS=${AMOUNT_OF_THREADS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ThreadPool)
endif()

if(BUILD_IMGUI)
  message(STATUS "[${PROJECT_NAME}] Adding imgui as subdirectory")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imgui)
  target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
endif()

message(STATUS "[${PROJECT_NAME}] Adding forces as subdirectory")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/forces)

message(STATUS "[${PROJECT_NAME}] Adding objects as subdirectory")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/physics)

message(STATUS "[${PROJECT_NAME}] Adding views as subdirectory")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/views)

target_link_libraries(${PROJECT_NAME} PUBLIC Forces Views Physics)
