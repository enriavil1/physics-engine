project(Forces)

# Collect header and source files using glob
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create a library target
add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

# Provide a clear message indicating the library creation
message(STATUS "[${PROJECT_NAME}] Library created")
