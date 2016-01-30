#ifndef TYPDEFINE_H
#define TYPDEFINE_H

#include <iostream>
//#define BIGDATA 1
#include <climits>

#ifndef NULL
#define NULL 0
#endif
#define BUFSIZE  1024
#define END      -1LL
#define ERRORFLAG  UULONG_MAX
#ifndef BIGDATA
 typedef unsigned long long UINT64;
 typedef UINT64                 kmer;
 const kmer ERRORFLAG =  ULLONG_MAX;

//used for kmer whose length larger than 28
//have not implemented
#else
 const size_t l = 8;
 typedef unsignedLargeInt<l>                kmer; //type for kmer
 const kmer ERRORFLAG        = kmer::max();
#endif

typedef uint32_t UINT32;
typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef size_t freq;


#endif // TYPDEFINE_H
