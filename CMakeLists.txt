cmake_minimum_required(VERSION 3.8)
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION_SUFFIX rc1)
project(cturtle VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

if (VERSION_SUFFIX)
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SUFFIX}")
else()
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)
link_libraries(Threads::Threads)

include(CMakeDependentOption)
option(ENABLE_TESTS "enable building of unit tests, spec tests" OFF)

# ##################################################################################################
# Check submodules and update.
# ##################################################################################################
find_package(Git QUIET)
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
   RESULT_VARIABLE GIT_SUBMOD_RESULT)

if(NOT GIT_SUBMOD_RESULT EQUAL "0")
   message(FATAL_ERROR "Failed to update submodules")
endif()

add_subdirectory(modules/cmake_bits)

# ##################################################################################################
# Add dependencies.
# ##################################################################################################
add_project_dependency(fmt
   REPO https://github.com/fmtlib/fmt
   TAG  master
   DIR  ${CMAKE_BINARY_DIR}
   ADD_SUBDIR "."
   SKIP_BUILD
   ARGS "-DENABLE_TESTS=Off"
)

add_library(cturtle INTERFACE)
target_include_directories(cturtle INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(bluegrass::cturtle ALIAS cturtle)
target_link_libraries(cturtle INTERFACE fmt::fmt-header-only)

if(ENABLE_TESTS)
   add_project_dependency(Catch2
      REPO https://github.com/catchorg/Catch2
      TAG  devel
      DIR  ${CMAKE_BINARY_DIR}
   )

   include(CTest)
   include(Catch)

   enable_testing()
   add_subdirectory(tests)
endif()
