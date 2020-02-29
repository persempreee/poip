#include <iostream>
#include "IVariable.hpp"
using namespace std;


class VOLT: public IVariable
{
public:
  VOLT(float K, float B) : IVariable(K,B, "Voltage")
  {
  }
};    