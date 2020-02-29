#include "adc1registers.hpp" //for ADC1
#include "adccommonregisters.hpp" //for ADCCommon
#include <iostream>
#include <array>

using namespace std;

enum class Resolution
{
  Bits12,
  Bits10,
  Bits8,
  Bits6
};

template <class Adc>
class ADC
{
private:
  inline static array<uint32_t, 19> channelsValue ={} ;
  inline static array<uint32_t, 19> channelsMap ={} ;
  
public:
  static void Start()
  {
    Adc::CR2::SWSTART::On::Set();
  }

  static uint32_t Get()
  {
    return Adc::DR::Get();
  }
  static bool IsReady()
  {
    return Adc::SR::EOC::ConversionComplete::IsSet();
  }
  static void On()
  {
    Adc::CR2::ADON::Enable::Set();
  }
  static void Off()
  {
    Adc::CR2::ADON::Disable::Set();
  }
  static void Config(Resolution resolution)
  {
    switch (resolution)
    {
    case Resolution::Bits12:
      Adc::CR1::RES::Bits12::Set();
      break;
      
    case Resolution::Bits10:
      Adc::CR1::RES::Bits10::Set();
      break;
      
    case Resolution::Bits8:
      Adc::CR1::RES::Bits8::Set();
      break;
      
    case Resolution::Bits6:
      Adc::CR1::RES::Bits6::Set();
      break;
     
    default:
      Adc::CR1::RES::Bits12::Set();
      break;
    }
  }
  
  static void ConfigChannel(uint32_t channelNum1, uint32_t channelNum2)
  {
    assert ((channelNum1 >= 0) && (channelNum1 <= 18)) ;
      assert ((channelNum2 >= 0) && (channelNum2 <= 18)) ;
    Adc::SQR1::L::Conversions2::Set();
    Adc::CR1::SCAN::Enable::Set();
    channelsMap[0] = channelNum1;
    channelsMap[1] = channelNum2;
    Adc::SQR3::SQ1::Set(channelNum1);
    Adc::SQR3::SQ2::Set(channelNum2);
  }
  
   static void Coder()
    {
      
       while (!IsReady());
      {
      };
        uint32_t data = Get();
        channelsValue[channelsMap[0]] = data ;
        
       while (!IsReady());
      {
      };   
        data = Get();
        channelsValue[channelsMap[1]] = data ;
        //cout<< "voltage:"<< dataV<<  " "<< "temperature:" << dataT<< endl;
        
    }
  
  static uint32_t GetChannelValue(uint32_t index)
  {
    return channelsValue[index] ;
  }
};    







//int n = 5
//int *n1 = new int[n]{0, 1, 2, 10, 18};