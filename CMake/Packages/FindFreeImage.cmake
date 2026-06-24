#snapped from: https://bitbucket.org/sinbad/ogre/src/0bba4f7cdb95/CMake/Packages/FindFreeImage.cmake?at=default
#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see httpwww.ogre3d.org
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find FreeImage
# Once done, this will define
#
#  FreeImage_FOUND - system has FreeImage
#  FreeImage_INCLUDE_DIRS - the FreeImage include directories 
#  FreeImage_LIBRARIES - link these to use FreeImage

include(FindPkgMacros)
findpkg_begin(FreeImage)

# Get path, convert backslashes as ${ENV_${var}}
getenv_path(FREEIMAGE_HOME)

# construct search paths
set(FreeImage_PREFIX_PATH ${FREEIMAGE_HOME} ${ENV_FREEIMAGE_HOME})
create_search_paths(FreeImage)
# redo search if prefix path changed
clear_if_changed(FreeImage_PREFIX_PATH
  FreeImage_LIBRARY_FWK
  FreeImage_LIBRARY_REL
  FreeImage_LIBRARY_DBG
  FreeImage_INCLUDE_DIR
)

set(FreeImage_LIBRARY_NAMES freeimage freeimageLib)
get_debug_names(FreeImage_LIBRARY_NAMES)

use_pkgconfig(FreeImage_PKGC freeimage)

findpkg_framework(FreeImage)

find_path(FreeImage_INCLUDE_DIR NAMES FreeImage.h HINTS ${FreeImage_INC_SEARCH_PATH} ${FreeImage_PKGC_INCLUDE_DIRS})

find_library(FreeImage_LIBRARY_REL NAMES ${FreeImage_LIBRARY_NAMES} HINTS ${FreeImage_LIB_SEARCH_PATH} ${FreeImage_PKGC_LIBRARY_DIRS} PATH_SUFFIXES  release relwithdebinfo minsizerel)
find_library(FreeImage_LIBRARY_DBG NAMES ${FreeImage_LIBRARY_NAMES_DBG} HINTS ${FreeImage_LIB_SEARCH_PATH} ${FreeImage_PKGC_LIBRARY_DIRS} PATH_SUFFIXES  debug)

make_library_set(FreeImage_LIBRARY)

findpkg_finish(FreeImage)

# On Windows with a static vcpkg build, FreeImage needs its transitive dependencies linked explicitly
if(FreeImage_FOUND AND WIN32 AND DEFINED VCPKG_TARGET_TRIPLET)
  set(_FI_VCPKG_LIB_DIR "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")
  foreach(_lib tiff openjp2 Imath-3_1 jpeg libpng16 zlib bz2 lzma jasper
               libwebp libwebpdemux libwebpmux libsharpyuv lcms2 raw_r
               OpenEXR-3_2 OpenEXRCore-3_2 Iex-3_2 IlmThread-3_2
               jpegxr jxrglue deflatestatic turbojpeg brotlidec brotlicommon)
    set(_lib_path "${_FI_VCPKG_LIB_DIR}/${_lib}.lib")
    if(EXISTS "${_lib_path}")
      list(APPEND FreeImage_LIBRARIES "${_lib_path}")
    endif()
  endforeach()
  unset(_FI_VCPKG_LIB_DIR)
  unset(_lib_path)
  unset(_lib)
endif()
