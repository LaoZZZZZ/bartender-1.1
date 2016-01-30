#ifndef REGULAREXPRESSION_H
#define REGULAREXPRESSION_H
#include <string>
#include <regex>
using std::regex;
using std::string;

string barcodeExtraction(const string& seq, const std::regex& pattern);
#endif // REGULAREXPRESSION_H
