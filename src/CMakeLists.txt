SET( NAME clyzer )

SET( SOURCES
  Clyzer.cpp
  Utility.cpp
  MainWindow.cpp
  )

SET( HEADERS
  Utility.h
  MainWindow.h
  )

SET( CORELIBS
     ${QT_LIBRARIES}
     )

QT4_WRAP_CPP( HEADERS_MOC ${HEADERS} )

ADD_EXECUTABLE( ${NAME} ${SOURCES} ${HEADERS_MOC} )
TARGET_LINK_LIBRARIES( ${NAME} ${CORELIBS} )
