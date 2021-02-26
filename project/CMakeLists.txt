cmake_minimum_required(VERSION 3.15)

function(download_file url filename hash_type hash)

if(NOT EXISTS ${filename})
  file(DOWNLOAD ${url} ${filename}
       TIMEOUT 60  # seconds
       EXPECTED_HASH ${hash_type}=${hash}
       TLS_VERIFY ON)
endif()

endfunction(download_file)

# Download and unpack googletest at configure time
configure_file(CMakeListsTest.txt googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

configure_file(CMakeListsBench.txt googlebenchmark-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-download )
if(result)
  message(FATAL_ERROR "CMake step for googlebenchmark failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-download )
if(result)
  message(FATAL_ERROR "Build step for googlebenchmark failed: ${result}")
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-build
                 EXCLUDE_FROM_ALL)

# Download cpplinq
download_file(https://codeplexarchive.blob.core.windows.net/archive/projects/cpplinq/cpplinq.zip ${CMAKE_CURRENT_BINARY_DIR}/cpplinq.zip SHA1 195fc180e34a78dd18751a1632d1567fc67d5b80)
file(ARCHIVE_EXTRACT INPUT ${CMAKE_CURRENT_BINARY_DIR}/cpplinq.zip)

project(main)
set(CMAKE_CXX_STANDARD $ENV{CPP_VERSION})
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

if(MSVC)
  if($ENV{IS_PERFORMANCE_TEST})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL /MP")
  set(CMAKE_CXX_FLAGS_DEBUG "/Od")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Oi -GL -Ot")
else()
  if(!$ENV{IS_PERFORMANCE_TEST})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wno-long-long -pedantic")
  endif()
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
endif()

file(GLOB
  SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/CppLinq/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CppLinq/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Test/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Test/*.cpp
    )
add_executable(main ${SOURCE_FILES})

target_include_directories(main PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/sourceCode/cpplinq/CppLinq ${CMAKE_CURRENT_SOURCE_DIR}/CppLinq)

if($ENV{IS_PERFORMANCE_TEST})
    add_definitions(-DENABLE_PERF)
endif()

target_link_libraries(main gtest)
target_link_libraries(main benchmark)

install(TARGETS main)

enable_testing()
add_test(NAME main COMMAND main)