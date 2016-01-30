#ifndef TYPDEFINE_H
#define TYPDEFINE_H

#include <iostream>
#include <climits>
#include <unordered_map>
#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif
#define BUFSIZE  1024
#define END      -1LL
#define ERRORFLAG  UULONG_MAX

typedef unsigned long long UINT64;
typedef UINT64                 kmer;
const kmer ERRORFLAG =  ULLONG_MAX;

typedef uint32_t UINT32;
typedef uint8_t UINT8;
typedef uint16_t UINT16;

typedef size_t freq;
typedef std::unordered_map<kmer, freq>        barcodeFreqTable;

#endif // TYPDEFINE_H
