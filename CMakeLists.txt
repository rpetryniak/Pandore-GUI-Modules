PROJECT( pandore-modules )

cmake_minimum_required(VERSION 2.6)
#include(CheckIncludeFiles)
#include(CheckTypeSize)

FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_USE_QTWEBKIT TRUE )
ADD_DEFINITIONS( -Wall )
INCLUDE( ${QT_USE_FILE} )

#PANDORE
FIND_PATH(PANDORE_DIR VERSION.txt
  /usr/include
  /usr/include/pandore
  /usr/share/pandore
  c:/pandore
  c:/program files/pandore
  $ENV{PANDORE_HOME}
)
SET ( PANDORE_INCLUDE
  ${PANDORE_DIR}/include
  ${PANDORE_DIR}/src/operatorsP0
)
IF(WIN32)
  SET ( PANDORE_LIB ${PANDORE_DIR}/lib/pandore.lib )
ENDIF(WIN32)
IF(UNIX)
  SET ( PANDORE_LIB ${PANDORE_DIR}/lib/libpandore.so )
ENDIF(UNIX)

#Region-merging module
SET( REGIONMERGING_SRCS app/main_regionmerging.cpp src/modules/regionmerging.cpp )
SET( REGIONMERGING_MOC_HDRS include/regionmerging.h )
SET( REGIONMERGING_UIS resources/ui/regionmerging.ui )
QT4_WRAP_UI( REGIONMERGING_UI_HDRS ${REGIONMERGING_UIS})
QT4_WRAP_CPP( REGIONMERGING_MOC_SRCS ${REGIONMERGING_MOC_HDRS} )

#Watershed module
SET( WATERSHED_SRCS app/main_watershed.cpp src/modules/watershed.cpp )
SET( WATERSHED_MOC_HDRS include/watershed.h )
SET( WATERSHED_UIS resources/ui/watershed.ui )
QT4_WRAP_UI( WATERSHED_UI_HDRS ${WATERSHED_UIS})
QT4_WRAP_CPP( WATERSHED_MOC_SRCS ${WATERSHED_MOC_HDRS} )

#Common definitions for all modules
SET( COMMON_SRCS src/gui/imagecanvas.cpp src/common/functions.cpp src/gui/helpwindow.cpp )
SET( COMMON_MOC_HDRS include/imagecanvas.h include/helpwindow.h )
SET( COMMON_UIS resources/ui/helpwindow.ui )
SET( COMMON_RCS resources/mdi.qrc )
QT4_WRAP_CPP( COMMON_MOC_SRCS ${COMMON_MOC_HDRS} )
QT4_WRAP_UI( COMMON_UI_HDRS ${COMMON_UIS} )
QT4_ADD_RESOURCES( COMMON_RC_SRCS ${COMMON_RCS} )

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/PandoreGuiModules src include ${PANDORE_INCLUDE} )

ADD_EXECUTABLE( pandore-merge_regions ${COMMON_SRCS} ${REGIONMERGING_SRCS} ${REGIONMERGING_MOC_SRCS} ${REGIONMERGING_UI_HDRS}
  ${COMMON_MOC_SRCS} ${COMMON_UI_HDRS} ${COMMON_RC_SRCS} )
ADD_EXECUTABLE( pandore-watershed ${WATERSHED_SRCS} ${WATERSHED_MOC_SRCS} ${WATERSHED_UI_HDRS}
  ${COMMON_SRCS} ${COMMON_MOC_SRCS} ${COMMON_UI_HDRS} ${COMMON_RC_SRCS} )

TARGET_LINK_LIBRARIES( pandore-merge_regions ${QT_LIBRARIES} ${PANDORE_LIB} )
TARGET_LINK_LIBRARIES( pandore-watershed ${QT_LIBRARIES} ${PANDORE_LIB} )
