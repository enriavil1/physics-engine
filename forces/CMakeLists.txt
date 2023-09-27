project(Forces)

file(GLOB HH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/**/*.hpp")
file(GLOB CXX_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")

add_library(${PROJECT_NAME} ${HH_FILES} ${CXX_FILES})
message(STATUS "[${PROJECT_NAME}] created library")
