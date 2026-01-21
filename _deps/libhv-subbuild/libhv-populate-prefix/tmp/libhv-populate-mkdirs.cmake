# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/funique0807/dev/GoPro_Controller/_deps/libhv-src")
  file(MAKE_DIRECTORY "/home/funique0807/dev/GoPro_Controller/_deps/libhv-src")
endif()
file(MAKE_DIRECTORY
  "/home/funique0807/dev/GoPro_Controller/_deps/libhv-build"
  "/home/funique0807/dev/GoPro_Controller/_deps/libhv-subbuild/libhv-populate-prefix"
  "/home/funique0807/dev/GoPro_Controller/_deps/libhv-subbuild/libhv-populate-prefix/tmp"
  "/home/funique0807/dev/GoPro_Controller/_deps/libhv-subbuild/libhv-populate-prefix/src/libhv-populate-stamp"
  "/home/funique0807/dev/GoPro_Controller/_deps/libhv-subbuild/libhv-populate-prefix/src"
  "/home/funique0807/dev/GoPro_Controller/_deps/libhv-subbuild/libhv-populate-prefix/src/libhv-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/funique0807/dev/GoPro_Controller/_deps/libhv-subbuild/libhv-populate-prefix/src/libhv-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/funique0807/dev/GoPro_Controller/_deps/libhv-subbuild/libhv-populate-prefix/src/libhv-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
