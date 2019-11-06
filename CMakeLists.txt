cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)
project(bcrypt VERSION 1.3.0 LANGUAGES C CXX)

file(GLOB headers CONFIGURE_DEPENDS include/*.hpp)
file(GLOB sources CONFIGURE_DEPENDS src/*.cpp src/*.h src/*.c)

add_library(bcrypt STATIC ${headers} ${sources})
target_compile_features(bcrypt PRIVATE cxx_std_20)
target_include_directories(bcrypt PRIVATE src PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
install(TARGETS bcrypt EXPORT bcrypt LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(EXPORT bcrypt FILE bcrypt-config.cmake NAMESPACE bcrypt:: DESTINATION lib/cmake/bcrypt)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/bcrypt-config-version.cmake
  VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/bcrypt-config-version.cmake DESTINATION lib/cmake/bcrypt)

add_library(bcrypt::bcrypt ALIAS bcrypt)
