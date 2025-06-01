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
set(CMAKE_C_FLAGS
    "${ABSL_DEFAULT_COPTS} -Ofast -Wall -Werror -Wno-ignored-attributes -Wno-maybe-uninitialized -Wno-nonnull"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Mbedtls
set(ENABLE_TESTING OFF)
add_subdirectory(./vendor/mbedtls)

# Use system protoc so that it works with the cross compiler
function(protobuf_generate_cpp LIBRARY)
  set(PROTOS ${ARGN})

  foreach(proto ${PROTOS})
    get_filename_component(PROTO_NAME "${proto}" NAME_WE)

    set(PROTO_HEADER "${PROTO_NAME}.pb.h")
    set(PROTO_SRC "${PROTO_NAME}.pb.cc")

    message(STATUS "Protobuf ${proto} -> ${PROTO_SRC} ${PROTO_HEADER}")

    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_SRC}"
             "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_HEADER}"
      COMMAND protoc ARGS --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
              -I${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${proto}
      WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
      DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${proto}"
      COMMENT "${proto} -> ${PROTO_SRC} ${PROTO_HEADER}")

    list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_SRC}")
    list(APPEND HEADERS "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_HEADER}")
  endforeach()

  if(MY_PROJECT_SHARED_LIBS)
    set(libType SHARED)
  else()
    set(libType)
  endif()

  add_library(${LIBRARY} STATIC ${SOURCES} ${HEADERS})
  target_compile_options(${LIBRARY} PRIVATE -std=c++11)
  target_link_libraries(${LIBRARY} PUBLIC protobuf)
  target_include_directories(${LIBRARY} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

# Protocol buffers
set(protobuf_BUILD_LIBPROTOC ON)
set(protobuf_FORCE_FETCH_DEPENDENCIES ON)

find_package(Protobuf CONFIG REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shard_common.proto
         ${CMAKE_CURRENT_BINARY_DIR}/room_state.proto
  COMMAND
    ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/proto/room_state.proto
    ${CMAKE_CURRENT_BINARY_DIR}/room_state.proto
  COMMAND
    ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/shard_common.proto
    ${CMAKE_CURRENT_BINARY_DIR}/shard_common.proto)

protobuf_generate_cpp(PROTO_LIB shard_common.proto room_state.proto)

set(SRC
    ./vendor/testing_h/ansi_colour.h
    ./vendor/testing_h/logger.c
    ./vendor/testing_h/logger.h
    ./vendor/testing_h/testing.c
    ./vendor/testing_h/testing.h
    ./vendor/testing_h/ansi_colour.h
    ./vendor/testing_h/logger.c
    ./vendor/testing_h/logger.h
    ./vendor/testing_h/testing.c
    ./vendor/testing_h/testing.h
    ./vendor/mongoose/mongoose.c
    ./vendor/mongoose/mongoose.h
    ./src/CTexture.h
    ./src/CTexture.cpp
    ./src/TextureManager.h
    ./src/TextureManager.cpp
    ./src/GameObject.h
    ./src/GameState.h
    ./src/GameStateBase.h
    ./src/GameStateBase.cpp
    ./src/ObjectParams.h
    ./src/Vector2D.h
    ./src/WebsocketClient.h
    ./src/WebsocketClient.cpp)

set(TEST)

set(LIBS mbedtls mbedx509 pthread ${ABSL_LIBS} ${Protobuf_LIBRARIES} PROTO_LIB)

if(WIIU)
  add_subdirectory(./vendor/SDL-wiiu)
else()
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)

  include_directories(project-shard-client ${SDL2_INCLUDE_DIRS}
                      ${SDL2_IMAGE_INCLUDE_DIRS})

  set(LIBS ${LIBS} ${SDL2_LIBRARIES} SDL2_image)
endif()

add_library(project-shard-client-lib OBJECT ${SRC})
target_link_libraries(project-shard-client-lib PRIVATE ${LIBS})
target_include_directories(
  project-shard-client-lib
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

add_executable(project-shard-client ./src/main.cpp)
target_link_libraries(project-shard-client ${LIBS} project-shard-client-lib)

add_executable(project-shard-client-test ./tests/main.cpp ${TEST})
target_link_libraries(project-shard-client-test ${LIBS}
                      project-shard-client-lib)
