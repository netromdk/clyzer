#include "Primality.h"

bool isPropPrime(const mpz_t n) {
  return mpz_probab_prime_p(n, 10) > 0;
}
  
bool isPropPrime(qint32 n) {
  mpz_t num;
  mpz_init(num);
  mpz_set_ui(num, n);
  bool propPrime = (mpz_probab_prime_p(num, 10) > 0);
  mpz_clear(num);
  return propPrime;
}
