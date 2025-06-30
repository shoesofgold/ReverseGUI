#prevent VS dev shell from using solution files by default
#in case -G is not used with cmake on Windows
set (CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)