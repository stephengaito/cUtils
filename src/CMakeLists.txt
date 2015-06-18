# This is the cmake description of how to build the CUtils
# sources as a subproject.

project(CUtils)

defineLibrary(cUtils "src" "" "src")
set_target_properties(cUtils PROPERTIES LINKER_LANGUAGE CXX)
