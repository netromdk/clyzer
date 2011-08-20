#ifndef CLYZER_RANDOM_H
#define CLYZER_RANDOM_H

#ifdef WIN32
#include <windows.h>
#include <Wincrypt.h>
#else
#include <fstream>
#endif

#include <QtGlobal>

class Random {
public:
  Random();
  ~Random();

  uchar getByte();
  quint32 getInt32();
  quint64 getInt64();

  quint64 getRange(quint64 start, quint64 end);

private:
#ifdef WIN32
  HCRYPTPROV hProv;
#else
  std::ifstream in;
#endif  
};

#endif // CLYZER_RANDOM_H
