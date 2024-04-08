# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Ropey_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Ropey_autogen.dir\\ParseCache.txt"
  "Ropey_autogen"
  )
endif()
