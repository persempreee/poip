#include <iostream>
#include "IVariable.hpp"
using namespace std;


class TEMP: public IVariable
{
public:
  TEMP(float K, float B) : IVariable(K,B, "Temperature")
  {
  }
};