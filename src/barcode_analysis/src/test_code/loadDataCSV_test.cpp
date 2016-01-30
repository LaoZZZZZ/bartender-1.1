#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include "../typdefine.h"
#include "../testSimulation.h"
using std::string;
using std::endl;
using std::cout;
using std::unique_ptr;
using barcodeSpace::loadDataFromCSV;
int main() {
    const string file = "../../simulation_data/Sequenced_time_point_1.csv";	
    barcodeFreqTable table;
    loadDataFromCSV(file, table);
    cout << table.size() << endl;
    return 0;
}
