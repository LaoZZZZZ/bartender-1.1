#ifndef HYPOTHESISTESTER_H
#define HYPOTHESISTESTER_H

namespace barcodeSpace {
class HypothesisTester
{
public:
    HypothesisTester();
    virtual bool isSignificant(double e1, double e2, double n1, double n2) = 0;
    virtual ~HypothesisTester() {}
};
}   // namespace barcodeSpace
#endif // HYPOTHESISTESTER_H
