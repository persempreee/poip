#ifndef IVAR_HPP
#define IVAR_HPP

class IVariable
{
private:
  float B2 = 0.0F;
  float K2 = 0.0F;
  float Value = 0.F; 
  const char* Name = "                      " ;
public:
  IVariable(float K, float B, const char* name): B2(B), K2(K), Name(name)
  {
    
  }
  
  const char* GetName()
  {
    return Name ;
  }
      
  
  virtual void Calculate(uint32_t data)
  {
    Value = data * K2 + B2;    
  }
  
  float GetValue() 
  {
    return Value;
  } ;
};    

#endif