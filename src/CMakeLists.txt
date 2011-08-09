SET( NAME clyzer )

SET( SOURCES
  Clyzer.cpp
  Mod.cpp 
  Utility.cpp
  Alphabet.cpp
  SubstitutionAlphabet.cpp
  Distribution.cpp
  MainWindow.cpp
  FrequencyDialog.cpp
  AffineDialog.cpp
  )

SET( HEADERS
  Mod.h
  Utility.h
  Alphabet.h
  SubstitutionAlphabet.h
  Distribution.h
  Affine.h
  MainWindow.h
  FrequencyDialog.h
  AffineDialog.h
  )

SET( CORELIBS
  ${QT_LIBRARIES}
  )

QT4_WRAP_CPP( HEADERS_MOC ${HEADERS} )

ADD_EXECUTABLE( ${NAME} ${SOURCES} ${HEADERS_MOC} )
TARGET_LINK_LIBRARIES( ${NAME} ${CORELIBS} )
