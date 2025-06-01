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

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_FLAGS "-Wall -Werror -Ofast -march=native ${ABSL_DEFAULT_COPTS} -Wno-ignored-attributes -Wno-maybe-uninitialized -Wno-nonnull -Wno-deprecated-declarations -Wno-sign-compare")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

# Mbedtls TODO: setup mbedtls 
# add_subdirectory(./vendor/mbedtls)

# Protocol buffers
set(ABSL_BUILD_TESTING OFF)
set(ABSL_ENABLE_INSTALL ON)
set(ABSL_PROPAGATE_CXX_STD ON)
add_subdirectory(./vendor/protobuf/)

find_package(absl)
set(ABSL_LIBS
    absl::base
    absl::core_headers
    absl::fixed_array
    absl::flat_hash_map
    absl::flat_hash_set
    absl::inlined_vector
    absl::optional
    absl::span
    absl::str_format
    absl::synchronization
    absl::algorithm
    absl::strings)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(PROTOS ./proto/shard_common.proto ./proto/room_state.proto)
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOS})
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS EXPORT_MACRO DLL_EXPORT ${PROTOS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS DESCRIPTORS PROTO_DESCS ${PROTOS})

set(SRC
    ./vendor/testing_h/ansi_colour.h ./vendor/testing_h/logger.c
    ./vendor/testing_h/logger.h ./vendor/testing_h/testing.c
    ./vendor/testing_h/testing.h

    ./vendor/testing_h/ansi_colour.h
    ./vendor/testing_h/logger.c
    ./vendor/testing_h/logger.h
    ./vendor/testing_h/testing.c
    ./vendor/testing_h/testing.h
    ./vendor/mongoose/mongoose.c
    ./vendor/mongoose/mongoose.h

    ./src/CTexture.h ./src/CTexture.cpp
    ./src/TextureManager.h ./src/TextureManager.cpp
    ./src/GameObject.h
    ./src/GameState.h
    ./src/GameStateBase.h ./src/GameStateBase.cpp
    ./src/ObjectParams.h
    ./src/Vector2D.h)

    ${PROTO_SRCS}
    ${PROTO_HDRS})

set(TEST)

set(LIBS mbedtls mbedcrypto mbedx509 pthread ${ABSL_LIBS} ${Protobuf_LIBRARIES})

if (WIIU) 
  add_subdirectory(./vendor/SDL-wiiu)
else() 
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  
  include_directories(project-shard-client ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

  set(LIBS ${LIBS} ${SDL2_LIBRARIES} SDL2_image)
endif()

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
