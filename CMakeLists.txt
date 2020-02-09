# A little bit rusty with CMake, here is a solution inspired by
# http://dominikberner.ch/cmake-interface-lib/  https://github.com/bernedom/SI 
# Only for convenience, Meson remains my default build system :)
#
cmake_minimum_required(VERSION 3.12)

project(
  "MissionImpossible"
  VERSION
  0.2.0
  DESCRIPTION
  "Named optional argument in C++17."
  HOMEPAGE_URL
  "https://github.com/vincent-picaud/MissionImpossible")

add_library(${PROJECT_NAME} INTERFACE)
# add alias so the project can be uses with add_subdirectory
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(GNUInstallDirs)
include(CTest)

# Adding the install interface generator expression makes sure that the include
# files are installed to the proper location (provided by GNUInstallDirs)
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

# locations are provided by GNUInstallDirs
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(EXPORT ${PROJECT_NAME}_Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

# install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/
#        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/ad_fwd.hpp
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/ad.hpp
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/always.hpp
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/complex_support.hpp
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/derivatives.hpp
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/functions.hpp
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/identity.hpp
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/MissionImpossible.hpp
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/mission_impossible_tape.hpp
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/tape.hpp
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/tape_vector.hpp
  ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/underlying_type.hpp

  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})


set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

include(CPack)

# examples/
#
add_subdirectory(examples/)

# test/
# TODO 
# if(BUILD_TESTING)
#   add_subdirectory(test)
# endif()
