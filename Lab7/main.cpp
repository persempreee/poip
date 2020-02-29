#include "ADC.hpp"
#include "Temp.hpp"
#include "Voltage.hpp"
#include "gpioaregisters.hpp"
#include "gpiocregisters.hpp" //
#include "rccregisters.hpp"
#include "tim2registers.hpp"
#include "adc1registers.hpp" //for ADC1
#include "adccommonregisters.hpp" //for ADCCommon
#include <iostream>

using namespace std;

extern "C"
{
int __low_level_init(void)
{
  //частота
  //Switch on external 8 MHz oscillator
  RCC::CR::HSEON::On::Set();
  while(!RCC::CR::HSERDY::Ready::IsSet())
  {};
  
  //Switch system clock on external oscillator
  RCC::CFGR::SW::Hse::Set();
  while (!RCC::CFGR::SWS::Hse::IsSet())
  {};
  
  //Switch on clock on PortA a
  RCC::AHB1ENR::GPIOAEN::Enable::Set();

  //Подключить АЦП к источнику тактирования – устанавливаем бит ADC1EN в регистре RCC::APB2ENR (АЦП тактируется от шины APB2).
  //Switch on clock on ADC1
  RCC::APB2ENR::ADC1EN::Enable::Set();
  
  //питание порта А
  RCC::AHB1ENR::GPIOAEN::Enable::Set();
  //порт А в аналоговом режиме
  GPIOA::MODER::MODER0::Analog::Set();
  
  //Сконфигурировать порты. Определиться по каким каналам будут проводиться измерения, затем соответствующие выводы портов настроить для работы в аналоговом режиме.

   //Switch On internal tempearture sensor  
  ADC_Common::CCR::TSVREFE::Enable::Set();
  
  //Сконфигурировать АЦП. 
  
  //Установить разрядность в регистре ADC::CR1
  //ADC1::CR1::RES::Bits12::Set();
   //Set single conversion mode
  //Установить режим одиночного преобразование в регистре ADC::CR1 (биты CONT и EOCS установить в нужное значение)
  ADC1::CR2::CONT::SingleConversion::Set();
  ADC1::CR2::EOCS::SingleConversion::Set();   
  
  // Set laentgh of conversion sequence to 1
  //Установить количество измерений 1 в регистре ADC1::SQR1 бит L
  //ADC1::SQR1::L::Conversions2::Set();
  // Connect first conversion on Channel 18 
  //Выбрать канал для первого преобразования в регистре ADC1::SQR3 биты SQ1
  //ADC1::SQR3::SQ2::Channel0::Set();
 //ADC1::SQR3::SQ1::Channel18::Set();
  // Set 84 (3) cycles sample rate for channel 18
  //Установить скорость дискретизации в регистре SMPRx для нужного канала
  ADC1::SMPR2::SMP0::Cycles84::Set();
  ADC1::SMPR1::SMP18::Cycles84::Set();
    //ADC1::CR1::SCAN::Enable::Set();
  //Включить АЦП. Это делается установкой бита ADON в регистре ADC::CR2.
  //ADC1::CR2::ADON::Enable::Set();
  
  return 1;
}
}

constexpr float B1 = 25.0F - 0.76F/0.0025F ;
constexpr float K1 = 3.3F/(4096.0F*0.0025F);

constexpr float B2 = 0;
constexpr float K2 = 3.3F/4096;

using MyADC = ADC<ADC1>;
//using MyTemp = TEMP<B1, K1> ;
//using MyVolt = VOLT<B2, K2> ;

constexpr uint32_t VoltChannel = 0U ;
constexpr uint32_t TempChannel = 18U ;

int main()
{
  MyADC::On();
  MyADC::Config(Resolution::Bits12);
  MyADC::ConfigChannel(VoltChannel, TempChannel);
  
  for (;;)
  {
  MyADC::Start();
  MyADC::Coder();
  TEMP temperature(K1, B1) ;
  VOLT voltage(K2, B2);
  IVariable* variables[2] = {&temperature, &voltage} ;
  variables[0]->Calculate(MyADC::GetChannelValue(TempChannel)) ;
  variables[1]->Calculate(MyADC::GetChannelValue(VoltChannel)) ;
  
  for (auto it: variables)
  {
    cout <<it->GetName() << ": " << it->GetValue() << endl ;
  }
  //temperature.Calculate(MyADC::GetChannelValue(TempChannel));
  //cout << temperature.GetValue() << endl ;
  //voltage.Calculate(MyADC::GetChannelValue(VoltChannel));
  //cout << voltage.GetValue() << endl ;
  //VOLT::GetVolt(MyADC::GetChannelValue(VoltChannel));
  
  //MyTemp::GetTemp();
  //MyVolt::GetVolt();
  
  };
}


