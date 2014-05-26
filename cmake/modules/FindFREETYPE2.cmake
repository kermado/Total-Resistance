#
# Try to find FreeType2 library and include path.
# Once done this will define
#
# FREETYPE2_FOUND
# FREETYPE2_INCLUDE_DIRS
# FREETYPE2_LIBRARY
#

FIND_PATH(FREETYPE2_INCLUDE_DIRS freetype.h
	/usr/local/include/freetype
	/usr/include/freetype
	/sw/include/freetype
	/opt/local/include/freetype
	${FREETYPE2_ROOT_DIR}/include/freetype
	DOC "The directory where freetype/ft2build.h resides")

# Prefer the static library.
FIND_LIBRARY(FREETYPE2_LIBRARY
	NAMES
		libfreetype.a
		libfreetype.so
		libfreetype.dylib
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		${FREETYPE2_ROOT_DIR}/lib
	DOC
		"The FreeType2 library")

SET(FREETYPE2_FOUND "NO")
IF(FREETYPE2_INCLUDE_DIRS AND FREETYPE2_LIBRARY)
	SET(FREETYPE2_LIBRARIES ${FREETYPE2_LIBRARY})
	SET(FREETYPE2_FOUND "YES")
	message(STATUS "Found FreeType2")

	# FreeType2 has some dependencies.
	IF(APPLE)
		MESSAGE(STATUS "Linking FreeType2 with libpng and bzip2...")
		find_library(libpng NAMES libpng png)
		find_library(zlib NAMES z)
		find_library(bzip2 NAMES bz2 libbz2 bzip2)
		SET(FREETYPE2_LIBRARIES ${FREETYPE2_LIBRARIES} ${libpng} ${zlib} ${bzip2})
	ELSEIF(UNIX)
		MESSAGE(STATUS "Linking FreeType2 with libpng...")
		find_library(libpng NAMES libpng png)
		SET(FREETYPE2_LIBRARIES ${FREETYPE2_LIBRARIES} ${libpng})
	ENDIF()
ENDIF()
