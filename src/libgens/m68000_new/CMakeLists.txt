PROJECT(m68k)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(m68kmake m68kmake.c)

add_custom_command(OUTPUT m68kops.c
	COMMAND m68kmake ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/m68k_in.c
	DEPENDS m68kmake m68k_in.c)

add_library(m68k STATIC m68kcpu.c m68kops.c m68kopac.c m68kopdm.c m68kopnz.c)
