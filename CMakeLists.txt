cmake_minimum_required(VERSION 3.5)

# Set extension name here
set(TARGET_NAME elephant_duck)

# DuckDB's extension distribution supports vcpkg. As such, dependencies can be added in ./vcpkg.json and then
# used in cmake with find_package. Feel free to remove or replace with other dependencies.
# Note that it should also be removed from vcpkg.json to prevent needlessly installing it..
find_package(OpenSSL REQUIRED)

set(EXTENSION_NAME ${TARGET_NAME}_extension)
set(LOADABLE_EXTENSION_NAME ${TARGET_NAME}_loadable_extension)

project(${TARGET_NAME})
include_directories(src/include hsrex)

file(GLOB HSREX_SOURCES hsrex/regcomp.c hsrex/regexec.c hsrex/regerror.c hsrex/regfree.c hsrex/regalone.c)
add_library(hsrex_static STATIC ${HSREX_SOURCES})
target_compile_options(hsrex_static PRIVATE -DREGEX_STANDALONE -fPIC -D_NDEBUG -O3 -DREGEX_WCHAR)

set(EXTENSION_SOURCES src/elephant_duck_extension.cpp src/pg_regex.cpp)

build_static_extension(${TARGET_NAME} ${EXTENSION_SOURCES})
build_loadable_extension(${TARGET_NAME} " " ${EXTENSION_SOURCES})

# Link OpenSSL in both the static library as the loadable extension
target_link_libraries(${EXTENSION_NAME} OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(${LOADABLE_EXTENSION_NAME} OpenSSL::SSL OpenSSL::Crypto)

target_link_libraries(${EXTENSION_NAME} hsrex_static)
target_link_libraries(${LOADABLE_EXTENSION_NAME} hsrex_static)

install(
  TARGETS hsrex_static
  EXPORT "${DUCKDB_EXPORT_SET}"
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")

install(
  TARGETS ${EXTENSION_NAME}
  EXPORT "${DUCKDB_EXPORT_SET}"
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")
