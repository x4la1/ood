# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\HarmonicVisualizer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\HarmonicVisualizer_autogen.dir\\ParseCache.txt"
  "HarmonicVisualizer_autogen"
  )
endif()
