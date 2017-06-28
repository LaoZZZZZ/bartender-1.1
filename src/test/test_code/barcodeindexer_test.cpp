#include "../barcodeindexer.hpp"


#include <string>
#include <vector>


using namespace std;
using namespace barcodeSpace;

int main() {
	string barcode = "AACAT";
        BarcodeIndexer indexer({0,2,4});
	cout << indexer.GetIndex(barcode) << endl;
	return 0;
}
