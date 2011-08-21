#include <cmath>

#include "Random.h"

Random::Random() {
#ifdef WIN32
  if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
    qCritical() << "Could not acquire PROV_RSA_FULL crypt context!";

    // Try creating a new default container.
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
      qCritical() << "Could not acquire even when creating a new default container!";
    }
  }
#else
  in.open("/dev/urandom");
  if (!in.is_open()) {
    qCritical() << "Could not open /dev/urandom!";
  }
#endif
}

Random::~Random() {
#ifdef WIN32
  CryptReleaseContext(hProv, 0);
#else
  in.close();
#endif
}  

uchar Random::getByte() {
#ifdef WIN32
  BYTE data[1];
  CryptGenRandom(hProv, 1, data);
  return data[0];
#else
  return in.get();
#endif
}

union char_int32 {
  char chars[4];
  quint32 n;
};

quint32 Random::getInt32() {
  char_int32 rand_int;  
#ifdef WIN32
  CryptGenRandom(hProv, 4, (BYTE*) rand_int.chars);
#else
  for (int i = 0; i < 4; i++) {
    rand_int.chars[i] = in.get();
  }
#endif  
  return rand_int.n;
}

union char_int64 {
  char chars[8];
  quint64 n;
};
                                 
quint64 Random::getInt64() {
  char_int64 rand_int; 
#ifdef WIN32
  CryptGenRandom(hProv, 8, (BYTE*) rand_int.chars);
#else 
  for (int i = 0; i < 8; i++) {
    rand_int.chars[i] = in.get();
  }
#endif
  return rand_int.n;
}

quint32 Random::getRange(quint32 start, quint32 end) {
  // Ignore if invalid input.
  if (start > end) {
    return 0;
  }

  quint32 res = getInt32();
  while (res < start || res > end) {
    if (res < start) {
      res += ceil((float) (end - start) / 4.0);
    }
      
    if (res > end) {
      res %= (end + 1);
    }
  }    
      
  return res;
}
