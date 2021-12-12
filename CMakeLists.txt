CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
# SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
option(EXAMPLE "shall we complie example" on)
#INCLUDE(build_for_r328)

include_directories(${CMAKE_SOURCE_DIR}/)

PROJECT(CppProgramer)

# SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_BUILD_TYPE "Release")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -Wall -g")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -Os -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++14 -O0 -Wall -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++14 -Os -Wall")

MESSAGE(STATUS ${CMAKE_SYSTEM_NAME})

# add_subdirectory(web)

set(EXEC_NAME touchscreen)

add_executable(${EXEC_NAME}
        main.cpp
        touchscreen.cpp
        xepoll.cpp
)

target_link_libraries(${EXEC_NAME} 

)


