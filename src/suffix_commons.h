/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef KMERS_COMMONS_H
#define KMERS_COMMONS_H

#include <fstream>
#include <inttypes.h>
#include <iostream>
#include <map>
#include <limits.h>
#include <list>
#include <memory>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <utility>
#include <vector>

#include "assert_helpers.h"
#include "formats.h"
#include "typdefine.h"
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

#endif // KMERS_COMMONS_H
