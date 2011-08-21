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
  Random.cpp
  alphabet/Alphabet.cpp
  alphabet/SubstitutionAlphabet.cpp
  analysis/Frequency.cpp
  analysis/Intervals.cpp
  analysis/IndexOfCoincidence.cpp
  analysis/Kasiski.cpp
  ciphers/Affine.cpp
  ciphers/Vigenere.cpp
  gui/MainWindow.cpp
  gui/FrequencyDialog.cpp
  gui/AffineDialog.cpp
  gui/VigenereDialog.cpp
  gui/KeywordMixedSequenceDialog.cpp
  gui/AlignInBlocksDialog.cpp
  math/Mod.cpp 
  math/GCD.cpp
  math/PollardRho.cpp 
  math/Primality.cpp
  )

SET( HEADERS
  Utility.h
  Random.h
  alphabet/Alphabet.h
  alphabet/SubstitutionAlphabet.h
  analysis/Frequency.h
  analysis/Intervals.h
  analysis/IndexOfCoincidence.h
  analysis/Kasiski.h
  ciphers/Affine.h
  ciphers/Vigenere.h
  gui/MainWindow.h
  gui/FrequencyDialog.h
  gui/AffineDialog.h
  gui/VigenereDialog.h
  gui/KeywordMixedSequenceDialog.h
  gui/AlignInBlocksDialog.h
  math/Mod.h
  math/GCD.h
  math/PollardRho.h
  math/Primality.h
  )

SET( CORELIBS
  ${QT_LIBRARIES}
  ${GMP_LIBRARIES}
  )

QT4_WRAP_CPP( HEADERS_MOC ${HEADERS} )

ADD_EXECUTABLE( ${NAME} ${SOURCES} ${HEADERS_MOC} )
TARGET_LINK_LIBRARIES( ${NAME} ${CORELIBS} )
