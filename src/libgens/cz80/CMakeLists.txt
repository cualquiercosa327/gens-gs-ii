PROJECT(cz80)

# TODO: Byteorder.

# cz80 library.
# FIXME: Set the PDB filename.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
ADD_LIBRARY(cz80 STATIC cz80.c cz80_reg.c)
