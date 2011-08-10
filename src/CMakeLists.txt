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
  alphabet/Alphabet.cpp
  alphabet/SubstitutionAlphabet.cpp
  analysis/Frequency.cpp
  analysis/Intervals.cpp
  gui/MainWindow.cpp
  gui/FrequencyDialog.cpp
  gui/AffineDialog.cpp
  math/Mod.cpp 
  )

SET( HEADERS
  Utility.h
  alphabet/Alphabet.h
  alphabet/SubstitutionAlphabet.h
  analysis/Frequency.h
  analysis/Intervals.h
  ciphers/Affine.h
  gui/MainWindow.h
  gui/FrequencyDialog.h
  gui/AffineDialog.h
  math/Mod.h
  )

SET( CORELIBS
  ${QT_LIBRARIES}
  )

QT4_WRAP_CPP( HEADERS_MOC ${HEADERS} )

ADD_EXECUTABLE( ${NAME} ${SOURCES} ${HEADERS_MOC} )
TARGET_LINK_LIBRARIES( ${NAME} ${CORELIBS} )
