#
# Try to find ASSIMP library and include path.
# Once done this will define
#
# ASSIMP_FOUND
# ASSIMP_INCLUDE_PATH
# ASSIMP_LIBRARY
#

FIND_PATH(ASSIMP_INCLUDE_PATH
	NAMES
		assimp
	PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		${ASSIMP_ROOT_DIR}/include
	DOC "The directory where /assimp resides")

# Prefer the static library.
FIND_LIBRARY(ASSIMP_LIBRARY
	NAMES
		libASSIMP.a
		libassimp.a
		libASSIMP.so
		libASSIMP.so.3
		libassimp.so
		libassimp.so.3
		libASSIMP.dylib
		libASSIMP.3.dylib
		libassimp.dylib
		libassimp.3.dylib
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		${ASSIMP_ROOT_DIR}/lib
	DOC
		"The ASSIMP library")

SET(ASSIMP_FOUND "NO")
IF(ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARY)
	SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
	SET(ASSIMP_FOUND "YES")
    message(STATUS "Found ASSIMP")

    # ASSIMP requires the Zlib library.
    MESSAGE(STATUS "Linking Zlib with ASSIMP...")
    FIND_LIBRARY(zlib NAMES z)
 	SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARIES} ${zlib})
ENDIF()
