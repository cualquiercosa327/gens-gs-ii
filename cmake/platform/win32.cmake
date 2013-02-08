# Win32-specific CFLAGS/CXXFLAGS.

# Set minimum Windows version to Windows 2000. (Windows NT 5.0)
SET(GENS_CFLAG_WIN32_WINNT "-D_WIN32_WINNT=0x0500")

# Debug flags.
IF(CMAKE_BUILD_TYPE MATCHES ^debug)
	SET(GENS_CFLAG_DEBUG "-ggdb")
ENDIF(CMAKE_BUILD_TYPE MATCHES ^debug)

# Initialize CFLAGS/CXXFLAGS.
SET(GENS_CFLAGS_PLATFORM "-fshort-wchar ${GENS_CFLAG_WIN32_WINNT} ${GENS_CFLAG_DEBUG}")
SET(GENS_CXXFLAGS_PLATFORM "-fshort-wchar ${GENS_CFLAG_WIN32_WINNT} ${GENS_CFLAG_DEBUG}")

# Test for static libgcc/libstdc++.
# NOTE: libstdc++ check removed for now.
SET(GENS_LDFLAGS_PLATFORM "")
FOREACH(FLAG_TEST "-static-libgcc" "-static-libstdc++" "-Wl,--large-address-aware" "-Wl,--nxcompat" "-Wl,--dynamicbase")
	# CMake doesn't like "+" characters in variable names.
	STRING(REPLACE "+" "_" FLAG_TEST_VARNAME "${FLAG_TEST}")

	CHECK_C_COMPILER_FLAG("${FLAG_TEST}" LDFLAG_${FLAG_TEST_VARNAME})
	IF(LDFLAG_${FLAG_TEST_VARNAME})
		SET(GENS_LDFLAGS_PLATFORM "${GENS_LDFLAGS_PLATFORM} ${FLAG_TEST}")
	ENDIF(LDFLAG_${FLAG_TEST_VARNAME})
	UNSET(LDFLAG_${FLAG_TEST_VARNAME})
	UNSET(FLAG_TEST_VARNAME)
ENDFOREACH()

# Enable windres support on MinGW.
# http://www.cmake.org/Bug/view.php?id=4068
IF(MINGW)
	SET(CMAKE_RC_COMPILER_INIT windres)
	ENABLE_LANGUAGE(RC)
	
	# NOTE: Setting CMAKE_RC_OUTPUT_EXTENSION doesn't seem to work.
	# Force windres to output COFF, even though it'll use the .res extension.
	SET(CMAKE_RC_OUTPUT_EXTENSION .obj)
	SET(CMAKE_RC_COMPILE_OBJECT
		"<CMAKE_RC_COMPILER> --output-format=coff <FLAGS> <DEFINES> -o <OBJECT> <SOURCE>")
ENDIF(MINGW)

# Unset variables we don't need elsewhere.
UNSET(GENS_CFLAG_WIN32_WINNT)
UNSET(GENS_CFLAG_DEBUG)