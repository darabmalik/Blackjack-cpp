# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\bj_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\bj_autogen.dir\\ParseCache.txt"
  "bj_autogen"
  )
endif()
