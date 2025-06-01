cmake_minimum_required(VERSION 3.9)

project(
  project-shard-client
  VERSION 1.0
  LANGUAGES C CXX)

# Clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(REPO_URL "https://github.com/pilksoc/project-shard-client")

message(STATUS "Building ${CMAKE_PROJECT_NAME}")
message(STATUS "Git repo: ${REPO_URL}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Version: ${CMAKE_PROJECT_VERSION}")
message(STATUS "Build OS: ${CMAKE_SYSTEM_NAME}")

# Get the branch name
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Commit info: ${GIT_COMMIT_HASH}@${GIT_BRANCH}")

# Version info
add_definitions("-DPSC_GIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")
add_definitions("-DPSC_GIT_BRANCH=\"${GIT_BRANCH}\"")
add_definitions(
  "-DPSC_VERSION=\"${CMAKE_PROJECT_VERSION} (${GIT_COMMIT_HASH}@${GIT_BRANCH})\""
)
add_definitions("-DPSC_OS=\"${CMAKE_SYSTEM_NAME}\"")
add_definitions("-DPSC_REPO_URL=\"${REPO_URL}\"")
add_definitions("-DPSC_PROJECT_VERSION=\"${CMAKE_PROJECT_VERSION}\"")
add_definitions("-DPSC_PROJECT_NAME=\"${CMAKE_PROJECT_NAME}\"")

# __FILENAME__
string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")
add_definitions("-D__FILENAME__=(&__FILE__[SOURCE_PATH_SIZE])")

set(CMAKE_C_FLAGS
    "-Wpedantic -Werror -Wno-unused-parameter -Wno-overlength-strings -Ofast -march=native  -Wno-deprecated-declarations -Wno-strict-prototypes -Wno-newline-eof"
)

set(SRC
    ./vendor/testing_h/ansi_colour.h ./vendor/testing_h/logger.c
    ./vendor/testing_h/logger.h ./vendor/testing_h/testing.c
    ./vendor/testing_h/testing.h

    ./src/CTexture.h ./src/CTexture.cpp
    ./src/TextureManager.h ./src/TextureManager.cpp
    ./src/GameObject.h
    ./src/GameState.h
    ./src/GameStateBase.h ./src/GameStateBase.cpp
    ./src/ObjectParams.h
    ./src/Vector2D.h)

set(TEST)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(project-shard-client ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

set(LIBS crypto mbedtls mbedcrypto mbedx509 ${SDL2_LIBRARIES} SDL2_image)

add_library(project-shard-client-lib OBJECT ${SRC})
target_link_libraries(project-shard-client-lib PRIVATE ${LIBS})
target_include_directories(
  project-shard-client-lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
                                  ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(project-shard-client ./src/main.cpp)
target_link_libraries(project-shard-client ${LIBS} project-shard-client-lib)

add_executable(project-shard-client-test ./tests/main.cpp ${TEST})
target_link_libraries(project-shard-client-test ${LIBS}
                      project-shard-client-lib)
