cmake_minimum_required(VERSION 2.8.9)
if(NOT ITK_SOURCE_DIR)
  include(itk-module-init.cmake)
endif()
project(ITKArrayFire)

#set(ITKArrayFire_LIBRARIES ITKArrayFire)

set(ITKArrayFire_SYSTEM_INCLUDE_DIRS ${ArrayFire_INCLUDE_DIRS})

set(ITKArrayFire_EXPORT_CODE_INSTALL "
set(ArrayFire_DIR \"${ArrayFire_DIR}\")
find_package(ArrayFire NO_MODULE REQUIRED)
")
set(ITKArrayFire_EXPORT_CODE_BUILD "
if(NOT ITK_BINARY_DIR)
  set(ArrayFire_DIR \"${ArrayFire_DIR}\")
  find_package(ArrayFire NO_MODULE REQUIRED)
endif()
")

if(NOT ITK_SOURCE_DIR)
  find_package(ITK REQUIRED)
  list(APPEND CMAKE_MODULE_PATH ${ITK_CMAKE_DIR})
  include(ITKModuleExternal)
else()
  itk_module_impl()
endif()
