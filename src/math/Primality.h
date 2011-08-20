#ifndef CLYZER_PRIMALITY_H
#define CLYZER_PRIMALITY_H

#include <gmp.h>
#include <QtGlobal>

bool isPropPrime(const mpz_t n);
bool isPropPrime(qint32 n);

#endif // CLYZER_PRIMALITY_H
