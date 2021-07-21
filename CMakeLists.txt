# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the MIT License

# Set cmake mimimun version
cmake_minimum_required(VERSION 3.8)

project(meshcat-cpp
  VERSION 0.0.1)

# Build shared libs
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(MSVC)
  set(CMAKE_DEBUG_POSTFIX "d")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(BUILD_SHARED_LIBS "Build libraries as shared as opposed to static" ON)

# Disable C and C++ compiler extensions.
# C/CXX_EXTENSIONS are ON by default to allow the compilers to use extended
# variants of the C/CXX language.
# However, this could expose cross-platform bugs in user code or in the headers
# of third-party dependencies and thus it is strongly suggested to turn
# extensions off.
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# add GNU dirs
include(GNUInstallDirs)

include(CMakePackageConfigHelpers)

option(ENABLE_RPATH "Enable RPATH for this library" ON)
mark_as_advanced(ENABLE_RPATH)
include(AddInstallRPATHSupport)
add_install_rpath_support(BIN_DIRS "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
  LIB_DIRS "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
  DEPENDS ENABLE_RPATH
  USE_LINK_PATH)

# Encourage user to specify a build type (e.g. Release, Debug, etc.), otherwise set it to Release.
if(NOT CMAKE_CONFIGURATION_TYPES)
  if(NOT CMAKE_BUILD_TYPE)
      message(STATUS "Setting build type to 'Release' as none was specified.")
      set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
  endif()
endif()

find_package(pybind11 CONFIG)
find_package(Eigen3 REQUIRED)

# Set default build type to "Release" in single-config generators
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, recommanded options are: Debug or Release" FORCE)
    endif()
    set(MESHCATCPP_BUILD_TYPES "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${MESHCATCPP_BUILD_TYPES})
endif()

set(LIBRARY_TARGET_NAME MeshcatCpp)

# List of CPP (source) library files.
set(${LIBRARY_TARGET_NAME}_SRC
  src/Visualizer.cpp)

set(${LIBRARY_TARGET_NAME}_HDR
  include/MeshcatCpp/Visualizer.h)

add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR} ${${LIBRARY_TARGET_NAME}_HDR_PRIVATE})
target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC Eigen3::Eigen)
target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE pybind11::embed)

add_library(MeshcatCpp::MeshcatCpp ALIAS MeshcatCpp)

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")

target_compile_features(${LIBRARY_TARGET_NAME} PUBLIC cxx_std_11)
target_compile_definitions(${LIBRARY_TARGET_NAME} PRIVATE "visibility=hidden")

# List exported CMake package dependencies
set(MESHCAT_CPP_EXPORTED_DEPENDENCIES "")
list(APPEND MESHCAT_CPP_EXPORTED_DEPENDENCIES "Eigen3 CONFIG")

install(TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT  ${PROJECT_NAME}
  COMPONENT runtime
  LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
  ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
  RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MeshcatCpp")

include(InstallBasicPackageFiles)
install_basic_package_files(${PROJECT_NAME}
  NAMESPACE MeshcatCpp::
  VERSION ${${PROJECT_NAME}_VERSION}
  COMPATIBILITY SameMajorVersion
  VARS_PREFIX ${PROJECT_NAME}
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  DEPENDENCIES ${MESHCAT_CPP_EXPORTED_DEPENDENCIES})

include(AddUninstallTarget)
