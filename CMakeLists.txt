cmake_minimum_required(VERSION 3.8)

project(cturtle)

add_subdirectory(modules/cmake_bits)

create_version(
   MAJOR 1
   MINOR 0
   PATCH 0
   SUFF  alpha
)

message( STATUS "Building cturtle v${cturtle_VERSION}..." )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)
link_libraries(Threads::Threads)

include(CMakeDependentOption)
option(ENABLE_INSTALL "enable installation" ON)
option(ENABLE_TESTS "enable building of unit tests" ON)

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
target_include_directories(cturtle INTERFACE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
add_library(bluegrass::cturtle ALIAS cturtle)
target_link_libraries(cturtle INTERFACE fmt::fmt-header-only)

#if(ENABLE_TESTS)
#   add_project_dependency(Catch2
#      REPO https://github.com/catchorg/Catch2
#      TAG  devel
#      DIR  ${CMAKE_BINARY_DIR}
#   )
#
#   include(CTest)
#   include(Catch)
#
#   enable_testing()
#   add_subdirectory(tests)
#endif()

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

   install(TARGETS cturtle fmt-header-only
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
