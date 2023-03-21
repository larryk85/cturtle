cmake_minimum_required(VERSION 3.10)

project(cturtle VERSION 1.0.0.0)

message( STATUS "Building cturtle v${cturtle_VERSION}..." )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)
link_libraries(Threads::Threads)

include(CMakeDependentOption)
option(CTURTLE_ENABLE_INSTALL "enable installation" ON)
option(CTURTLE_ENABLE_TESTS "enable building of unit tests" OFF)

find_package(fmt REQUIRED)

if (NOT fmt_FOUND)
   include(FetchContent)
   # ##################################################################################################
   # Add dependencies.
   # ##################################################################################################
   FetchContent_Declare(
      fmt
      GIT_REPOSITORY https://github.com/fmtlib/fmt
      GIT_TAG master
   )

   FetchContent_GetProperties(fmt)
   if (NOT fmt_POPULATED)
      set(FMT_INSTALL ON)
      FetchContent_MakeAvailable(fmt)
   endif()
endif()

add_library(cturtle INTERFACE)
target_include_directories(cturtle INTERFACE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
add_library(bluegrass::cturtle ALIAS cturtle)
target_link_libraries(cturtle INTERFACE fmt::fmt-header-only)

if(ENABLE_TESTS)
   FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2
      GIT_TAG devel
   )
   
   FetchContent_GetProperties(Catch2)
   if (NOT Catch2_POPULATED)
      FetchContent_MakeAvailable(Catch2)
      list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/contrib") 
   endif()

   include(CTest)
   include(Catch)

   enable_testing()
   add_subdirectory(tests)
endif()

# ##################################################################################################
# Configure version info.
# ##################################################################################################
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/modules/cturtle-config-version.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/cturtle-config-version.cmake @ONLY)

# ##################################################################################################
# Installation.
# ##################################################################################################
if(ENABLE_INSTALL)
   include(CMakePackageConfigHelpers)
   include(GNUInstallDirs)

   message(STATUS "Installing bluegrass cturtle ...")
   set(cturtle_INCLUDE_DIRS ${CMAKE_INSTALL_INCLUDEDIR})
   configure_package_config_file(modules/cturtle-config.cmake.in
      ${CMAKE_CURRENT_BINARY_DIR}/cturtle-config.cmake
      INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cturtle
      PATH_VARS cturtle_INCLUDE_DIRS
   )
   write_basic_package_version_file(
      ${CMAKE_CURRENT_BINARY_DIR}/cturtle-config-version.cmake
      VERSION ${cturtle_VERSION}
      COMPATIBILITY SameMajorVersion
   )

   install(TARGETS cturtle
           EXPORT cturtle-targets
           LIBRARY
           PUBLIC_HEADER
              DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
              COMPONENT Headers
   )
   install(EXPORT cturtle-targets
      NAMESPACE bluegrass::
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cturtle
   )
   install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/bluegrass
           DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
   )
   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cturtle-config.cmake
                 ${CMAKE_CURRENT_BINARY_DIR}/cturtle-config-version.cmake
                 DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cturtle
   )
endif()
