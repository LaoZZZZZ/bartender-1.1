#include "../barcodeeditor.hpp"
#include "../kmerdecoder.hpp"

#include <string>
#include <vector>
#include <iostream>


using namespace std;
using namespace barcodeSpace;
int main() {
	vector<pair<string, freq>> barcodes = {
	{"GCAAAAACGTCCTCA", 5},
	{"CCCCCAAGCACCCGT", 4},
	{"GCGGGAATACCCGAC", 3},
	{"GTTTTAAATGCAATG", 3},
	{"GATAGAACCTTCCGA", 2}};
	BarcodeEditor editor(10);
	editor.editBarcodes(barcodes);
	const vector<barcodeFreqTable>& btables = editor.barcodeTable();
	for (size_t len = 0; len < btables.size(); ++len) {
		if (btables[len].size()) {	
			cout << len << ":" << endl;
			KmerDecoder decoder(len);
			for (const auto& b : btables[len]) {
				cout << b.first << '\t' <<  decoder.DNASequence(b.first) << '\t' << b.second << endl;
			}
		}
	}
	const vector<BarcodeEditor::BarcodeSequenceTable>& bstables = editor.barcode2Sequence();
	for (size_t len = 0; len < bstables.size(); ++len) {
		if (bstables[len].size()) {	
			cout << len << ":" << endl;
			KmerDecoder decoder(len);
			for (const auto& b : bstables[len]) {
				cout << decoder.DNASequence(b.first) << '\t' << b.second.front()._rawdata << '\t' << b.second.front()._lineNumber << endl;
			}
		}
	}

	cout << btables.size() << '\t' << bstables.size() << endl; 
	return 0;
}
