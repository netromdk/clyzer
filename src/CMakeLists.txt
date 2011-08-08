SET( NAME clyzer )

SET( SOURCES
  Clyzer.cpp
  Utility.cpp
  Distribution.cpp
  MainWindow.cpp
  FrequencyDialog.cpp
  )

SET( HEADERS
  Utility.h
  Distribution.h
  MainWindow.h
  FrequencyDialog.h
  )

SET( CORELIBS
  ${QT_LIBRARIES}
  )

QT4_WRAP_CPP( HEADERS_MOC ${HEADERS} )

ADD_EXECUTABLE( ${NAME} ${SOURCES} ${HEADERS_MOC} )
TARGET_LINK_LIBRARIES( ${NAME} ${CORELIBS} )
