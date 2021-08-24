cmake_minimum_required(VERSION 3.10)

project(RaytracingWeekend)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB HEADER_FILES *.h)
file(GLOB SOURCE_FILES *.cpp)

add_executable(
	${CMAKE_PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

#No zero-check
set(CMAKE_SUPPRESS_REGENERATION true)