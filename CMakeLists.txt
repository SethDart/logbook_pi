##---------------------------------------------------------------------------
## Author:      Jean-Eudes Onfray
## Copyright:   
## License:     wxWidgets License
##---------------------------------------------------------------------------


# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)
# This should be 2.8.0 to have FindGTK2 module
IF (COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 OLD)
  CMAKE_POLICY(SET CMP0005 OLD)
  CMAKE_POLICY(SET CMP0011 OLD)
ENDIF (COMMAND cmake_policy)

PROJECT(logbook_pi)

SET(PACKAGE_NAME logbook_pi)
SET(PLUGIN_SOURCE_DIR .)
MESSAGE (STATUS "*** Building ${PACKAGE_NAME} ***")

#SET(CMAKE_BUILD_TYPE Debug)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

# require proper c++
#ADD_DEFINITIONS( "-Wall -ansi -pedantic -Wno-variadic-macros" )
#TODO: Should we use  -fno-stack-protector
#  IF NOT DEBUGGING CFLAGS="-O2 -march=native"
IF(NOT WIN32)
 ADD_DEFINITIONS( "-Wall -g -fexceptions -fvisibility=hidden" )

 IF(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
 ELSE(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl")
 ENDIF(NOT APPLE)

ENDIF(NOT WIN32)

#SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")

# Add some definitions to satisfy MS
IF(WIN32)
    ADD_DEFINITIONS(-D__MSVC__)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
ENDIF(WIN32)


#SET(wxWidgets_USE_LIBS base core net xml html adv aui)
#SET(BUILD_SHARED_LIBS TRUE)
#SET( wxWidgets_USE_DEBUG OFF)
#SET( wxWidgets_USE_UNICODE ON)
#FIND_PACKAGE(wxWidgets REQUIRED)

INCLUDE(${wxWidgets_USE_FILE})

FIND_PACKAGE(Gettext REQUIRED)
FIND_PACKAGE(TinyXML)

# For convenience we define the sources as a variable. You can add
# header files and cpp/c files and CMake will sort them out


SET(SRC_LOGBOOK
            src/logbook_pi.h
 		src/logbook_pi.cpp
            src/icons.cpp
            src/icons.h
 	)

SET(SRC_NMEA0183
		src/nmea0183/latlong.hpp
		src/nmea0183/latlong.cpp
		src/nmea0183/long.cpp
		src/nmea0183/nmea0183.cpp
		src/nmea0183/nmea0183.hpp
		src/nmea0183/response.hpp
		src/nmea0183/response.cpp
		src/nmea0183/rmb.hpp
		src/nmea0183/rmb.cpp
		src/nmea0183/sentence.hpp
		src/nmea0183/sentence.cpp
		src/nmea0183/talkerid.cpp
		src/nmea0183/rmc.hpp
		src/nmea0183/rmc.cpp
		src/nmea0183/hexvalue.cpp
		src/nmea0183/lat.cpp
		src/nmea0183/expid.cpp
		src/nmea0183/wpl.hpp
		src/nmea0183/wpl.cpp
		src/nmea0183/rte.hpp
		src/nmea0183/rte.cpp
		src/nmea0183/hdt.hpp
		src/nmea0183/hdt.cpp
		src/nmea0183/hdg.hpp
		src/nmea0183/hdg.cpp
		src/nmea0183/hdm.hpp
		src/nmea0183/hdm.cpp
		src/nmea0183/gll.hpp
		src/nmea0183/gll.cpp
		src/nmea0183/vtg.hpp
		src/nmea0183/vtg.cpp
		src/nmea0183/gga.hpp
		src/nmea0183/gga.cpp
            src/nmea0183/dbt.cpp
            src/nmea0183/dbt.hpp
            src/nmea0183/dpt.cpp
            src/nmea0183/dpt.hpp
            src/nmea0183/mtw.cpp
            src/nmea0183/mtw.hpp
            src/nmea0183/mwv.cpp
            src/nmea0183/mwv.hpp
            src/nmea0183/vhw.hpp
            src/nmea0183/vhw.cpp
            src/nmea0183/vwr.cpp
            src/nmea0183/vwr.hpp
            src/nmea0183/zda.cpp
            src/nmea0183/zda.hpp
            src/nmea0183/mwd.cpp
            src/nmea0183/mwd.hpp
            src/nmea0183/vwt.cpp
            src/nmea0183/vwt.hpp
      )

INCLUDE_DIRECTORIES(src/nmea0183)

SET(SRCS ${SRC_LOGBOOK} ${SRC_NMEA0183})

SET(SRC_TINYXML
            src/tinyxml.cpp
            src/tinyxml.h
            src/tinyxmlerror.cpp
            src/tinyxmlparser.cpp
	)

IF(TINYXML_FOUND)
    message (STATUS "Building with tinyxml includes")
    INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
    ADD_DEFINITIONS(-DTIXML_USE_STL)
ELSE(TINYXML_FOUND)
    message (STATUS "tinyxml library not found")
    SET(SRCS ${SRCS} ${SRC_TINYXML})
    ADD_DEFINITIONS(-DTIXML_USE_STL)
ENDIF(TINYXML_FOUND)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} )
IF(TINYXML_FOUND)
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} ${TINYXML_LIBRARIES} )
ENDIF(TINYXML_FOUND)

IF(WIN32)
  SET(OPENCPN_IMPORT_LIB "../../${CMAKE_CFG_INTDIR}/${PARENT}")
  TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${OPENCPN_IMPORT_LIB} )
  ADD_DEPENDENCIES(${PACKAGE_NAME} ${PARENT})
ENDIF(WIN32)

TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${wxWidgets_LIBRARIES} )

IF(UNIX)
INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME LIBRARY DESTINATION ${PREFIX_PLUGINS})
ENDIF(UNIX)

IF(WIN32)
INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME DESTINATION "plugins")
ENDIF(WIN32)

# find src/ -name \*.cpp -or -name \*.h -or -name \*.hpp >po/POTFILES.in
FIND_PROGRAM(GETTEXT_XGETTEXT_EXECUTABLE xgettext)
IF (GETTEXT_XGETTEXT_EXECUTABLE)
ADD_CUSTOM_COMMAND(
   OUTPUT po/${PACKAGE_NAME}.pot.dummy
   COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} --force-po --package-name=${PACKAGE_NAME} --package-version="${PACKAGE_VERSION}" --output=po/${PACKAGE_NAME}.pot  --keyword=_ --width=80 --files-from=${CMAKE_CURRENT_SOURCE_DIR}/po/POTFILES.in
   DEPENDS po/POTFILES.in po/${PACKAGE_NAME}.pot
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
   COMMENT "logbook-pot-update [${PACKAGE_NAME}]: Generated pot file."
)
ADD_CUSTOM_TARGET(logbook-pot-update COMMENT "logbook-pot-update: Done." DEPENDS po/${PACKAGE_NAME}.pot.dummy)

ENDIF(GETTEXT_XGETTEXT_EXECUTABLE )

MACRO(GETTEXT_UPDATE_PO _potFile)
   SET(_poFiles ${_potFile})
   GET_FILENAME_COMPONENT(_absPotFile ${_potFile} ABSOLUTE)

   FOREACH (_currentPoFile ${ARGN})
      GET_FILENAME_COMPONENT(_absFile ${_currentPoFile} ABSOLUTE)
      GET_FILENAME_COMPONENT(_poBasename ${_absFile} NAME_WE)

      ADD_CUSTOM_COMMAND(
         OUTPUT ${_absFile}.dummy
         COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --width=80 --strict --quiet --update --backup=none --no-location -s ${_absFile} ${_absPotFile}
         DEPENDS ${_absPotFile} ${_absFile}
         COMMENT "logbook-po-update [${_poBasename}]: Updated po file."
      )

      SET(_poFiles ${_poFiles} ${_absFile}.dummy)

   ENDFOREACH (_currentPoFile )

   ADD_CUSTOM_TARGET(logbook-po-update COMMENT "logbook-po-update: Done." DEPENDS ${_poFiles})
ENDMACRO(GETTEXT_UPDATE_PO)

IF (GETTEXT_MSGMERGE_EXECUTABLE)
FILE(GLOB PACKAGE_PO_FILES po/*.po)
GETTEXT_UPDATE_PO(po/${PACKAGE_NAME}.pot ${PACKAGE_PO_FILES})
ENDIF(GETTEXT_MSGMERGE_EXECUTABLE)

SET(_gmoFiles)
MACRO(GETTEXT_BUILD_MO)
   FOREACH (_poFile ${ARGN})
      GET_FILENAME_COMPONENT(_absFile ${_poFile} ABSOLUTE)
      GET_FILENAME_COMPONENT(_poBasename ${_absFile} NAME_WE)
      SET(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_poBasename}.mo)

      ADD_CUSTOM_COMMAND(
         OUTPUT ${_gmoFile}
         COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --check -o ${_gmoFile} ${_absFile}
         DEPENDS ${_absFile}
         COMMENT "logbook-i18n [${_poBasename}]: Created mo file."
      )

      INSTALL(FILES ${_gmoFile} DESTINATION ${PREFIX_DATA}/locale/${_poBasename}/LC_MESSAGES RENAME opencpn-${PACKAGE_NAME}.mo )
      SET(_gmoFiles ${_gmoFiles} ${_gmoFile})
   ENDFOREACH (_poFile )
ENDMACRO(GETTEXT_BUILD_MO)

if(GETTEXT_MSGFMT_EXECUTABLE)
   FILE(GLOB PACKAGE_PO_FILES po/*.po)
   GETTEXT_BUILD_MO(${PACKAGE_PO_FILES})
   ADD_CUSTOM_TARGET(logbook-i18n COMMENT "logbook-i18n: Done." DEPENDS ${_gmoFiles})
   #ADD_DEPENDENCIES(logbook-i18n ${PACKAGE_PO_FILES})
   ADD_DEPENDENCIES(${PACKAGE_NAME} logbook-i18n)
ENDIF(GETTEXT_MSGFMT_EXECUTABLE)

