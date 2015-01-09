project(homesec_witty)
cmake_minimum_required(VERSION 2.8.3)
INCLUDE(CPack)

SET(MAJOR_VERSION "0")
SET(MINOR_VERSION "0")
SET(PATCH_VERSION "1")

#FIND_PACKAGE( Qt4 )
#FIND_PACKAGE( Wt REQUIRED )

SET (WT_CONNECTOR "wthttp" CACHE STRING "Connector used (wthttp or wtfcgi)")


add_subdirectory(src)
