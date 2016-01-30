/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef PATTERN_H
#define PATTERN_H
#include "patternparser.h"
#include "fastapattern.h"
#include "fastqpattern.h"
#include "formats.h"
#include <string>
namespace barcodeSpace {
patternParser* CreatePatternParser(const std::string& filename, file_format format);
}
#endif // PATTERN_H
