
#ifndef TempVoltLib_H_
#define TempVoltLib_H_
#include<string>
using std::string;

namespace liba{
class TempVoltLib {

public:
	TempVoltLib();
	virtual ~TempVoltLib();
	float** readTempValue(unsigned , unsigned);
	float tempToVoltage(float**,int, float);
	double calculateVoltageFromResistance(double);
};
}
#endif
