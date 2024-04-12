# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Text-Editor-Using-Rope_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Text-Editor-Using-Rope_autogen.dir\\ParseCache.txt"
  "Text-Editor-Using-Rope_autogen"
  )
endif()
