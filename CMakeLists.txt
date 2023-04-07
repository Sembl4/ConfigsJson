cmake_minimum_required(VERSION 3.12)
project(example)

set(CMAKE_CXX_STANDARD 20)

# Flags
if(CMAKE_BUILD_TYPE MATCHES "Debug")
	add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
	add_definitions(-DRELEASE)
endif()

if(CMAKE_OPTIMIZE MATCHES "O3")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS "-O3")
elseif(CMAKE_OPTIMIZE MATCHES "O2")
	set(CMAKE_CXX_FLAGS_RELEASE "-O2")
	set(CMAKE_CXX_FLAGS "-O2")
elseif(CMAKE_OPTIMIZE MATCHES "O1")
	set(CMAKE_CXX_FLAGS_RELEASE "-O1")
	set(CMAKE_CXX_FLAGS "-O1")
elseif(CMAKE_OPTIMIZE MATCHES "None")
endif()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_subdirectory(lib)

include_directories(
    /usr/include/nlohmann
	${PROJECT_SOURCE_DIR}/examples
    ${PROJECT_SOURCE_DIR}/lib
)

add_executable(${PROJECT_NAME} examples/example.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Config
)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}/examples"
    "${PROJECT_SOURCE_DIR}/lib"
)