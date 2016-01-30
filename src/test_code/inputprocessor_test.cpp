#include "../inputprocessor.hpp"
#include "../rawbarcodeprocessor.hpp"
#include "../countbarcodeprocessor.hpp"

using namespace std;
using namespace barcodeSpace;

int main() {
	InputProcessor* processor = new RawBarcodeProcessor("raw_barcode.txt", false);
	processor->process();
	vector<std::pair<std::string, freq>> sequence = processor->BarcodeList();	
	for (const auto& p : sequence) {
		cout << p.first << '\t' << p.second << endl;
	}
	delete processor;
	processor = new CountBarcodeProcessor("count_barcode.txt", false);
	processor->process();
	sequence = processor->BarcodeList();	
	for (const auto& p : sequence) {
		cout << p.first << '\t' << p.second << endl;
	}
	return 0;
}
