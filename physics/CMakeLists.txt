project(Physics)

# Collect header and source files using glob
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create a library target
add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

# Provide a clear message indicating the library creation
message(STATUS "[${PROJECT_NAME}] Library created")

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)

# Set include directories for the library (if needed)
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                         $<INSTALL_INTERFACE:include>)
