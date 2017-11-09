PROJECT(c68k)

# NASM requires CMake 2.8.4.
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)

# Sources.
SET(c68k_SRCS
	c68k.c
	c68kexec.c
	gen68k.c
	)

set(HEADER_FILES
	c68k.h
	core.h
	gen68k.h
	gen68k.inc
	c68kmac.inc
	c68k_ini.inc
	c68k_opF.inc
	c68k_op0.inc
	c68k_op1.inc
	c68k_op2.inc
	c68k_op3.inc
	c68k_op4.inc
	c68k_op5.inc
	c68k_op6.inc
	c68k_op7.inc
	c68k_op8.inc
	c68k_op9.inc
	c68k_opA.inc
	c68k_opB.inc
	c68k_opC.inc
	c68k_opD.inc
	c68k_opE.inc
	c68k_opF.inc
	)

######################
# Build the library. #
######################

ADD_LIBRARY(c68k STATIC ${c68k_SRCS})
INCLUDE(SetMSVCDebugPath)
SET_MSVC_DEBUG_PATH(mameZ80)
