SET( NAME clyzer )

INCLUDE_DIRECTORIES(
  alphabet
  analysis
  ciphers
  gui
  math
  )

SET( SOURCES
  Clyzer.cpp
  Utility.cpp
  MainWindow.cpp
  math/Mod.cpp 
  alphabet/Alphabet.cpp
  alphabet/SubstitutionAlphabet.cpp
  analysis/Distribution.cpp
  gui/FrequencyDialog.cpp
  gui/AffineDialog.cpp
  )

SET( HEADERS
  Utility.h
  MainWindow.h
  math/Mod.h
  alphabet/Alphabet.h
  alphabet/SubstitutionAlphabet.h
  analysis/Distribution.h
  ciphers/Affine.h
  gui/FrequencyDialog.h
  gui/AffineDialog.h
  )

SET( CORELIBS
  ${QT_LIBRARIES}
  )

QT4_WRAP_CPP( HEADERS_MOC ${HEADERS} )

ADD_EXECUTABLE( ${NAME} ${SOURCES} ${HEADERS_MOC} )
TARGET_LINK_LIBRARIES( ${NAME} ${CORELIBS} )
