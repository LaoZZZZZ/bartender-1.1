
#include "../clustermergeronesampletester.hpp"
#include <iostream>
#include <vector>
#include <array>
using namespace std;
using namespace barcodeSpace;

int main() {
	double zvalue = 3.08; // p-value = 0.001
	ClusterMergerOneSampleTester tester(zvalue, 0.002);
       const vector<array<int,4>> frequency_table_1(
	{{20,0 , 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{0, 0, 0, 20}});
    const vector<array<int,4>> frequency_table_2(
	{{0,20 , 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{0, 0, 0, 20}});
    const vector<array<int,4>> frequency_table_3(
	{{0,2 , 0, 0},
	{2, 0, 0, 0},
	{2, 0, 0, 0},
	{2, 0, 0, 0},
	{0, 0, 0, 2}});
    const vector<array<int,4>> frequency_table_4(
	{{0,200, 0, 0},
	{200, 0, 0, 0},
	{200, 0, 0, 0},
	{200, 0, 0, 0},
	{0, 0, 0, 200}});
	std::cout << tester.shouldMerge(frequency_table_4, frequency_table_1) << std::endl; 
	std::cout << tester.shouldMerge(frequency_table_4, frequency_table_3) << std::endl;

       const vector<array<int,4>> frequency_table_5(
	{{17,3 , 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{0, 0, 0, 20}});

	
	std::cout << tester.shouldMerge(frequency_table_5, frequency_table_2) << std::endl; 

	return 0;
}
