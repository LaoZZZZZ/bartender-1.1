/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef KMERS_COMMONS_H
#define KMERS_COMMONS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <stdint.h>
#include <inttypes.h>
#include <memory>
#include <string.h>
#include <sstream>
#include <limits.h>
#include <utility>
#include "assert_helpers.h"
#include "formats.h"
#include "typdefine.h"
#include "kmers_hashset.h"
using  std::string;
using  std::map;
using  std::list;
using  std::vector;


using std::shared_ptr;
using std::unique_ptr;
using  std::exception;
using  std::out_of_range;
using  std::invalid_argument;
using  std::bad_alloc;
using  std::logic_error;
using  std::stringstream;
using  std::fstream;

using  std::make_pair;
using  std::cin;
using  std::cout;
using  std::cerr;
using  std::endl;

typedef prefixMatching::kmers_sparseHash<kmer,freq>         BarcodeTable;

#endif // KMERS_COMMONS_H
