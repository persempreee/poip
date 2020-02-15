//#include <cstdint>            //for int types such as uint32_t
#include "gpiocregisters.hpp" //for Gpioc
#include "gpiacregisters.hpp" //for Gpioa
#include "rccregisters.hpp"   //for RCC
#include "tim5registers.hpp"   //for SPI5
#include "nvicregisters.hpp"  //for NVIC

using namespace std ;



extern "C"
{
int __low_level_init(void)
{
  RCC::CR::HSION::On::Set();
  while (!RCC::CR::HSIRDY::Ready::IsSet());
  {}
  
  RCC::CFGR::SW::Hsi::Set();
  RCC::CFGR::SWS::Hsi::IsSet();
  while (!RCC::CFGR::SW::Hsi::IsSet());
  {}
  RCC::APB1ENR::TIM5EN::Enable::Set() ; //включаем таймер2 к тактированию
  
  TIM5::CR1::URS::Value1::Set(); //только переполнение генерирует обновление прерывания
  
  RCC::AHB1ENR::GPIOCEN::Enable::Set() ; //подключение к системе тактирования джпиоц
  GPIOC::MODER::MODER5::Output::Set() ; //настраиваем порт С5 на выход
  
  TIM5::PSC::Write(15999); //деление на 16000 (15999+!)
  NVIC::ISER1::Write(1<<18); //разрешение глобальных прерываний
  TIM5::ARR::Write(2000); //время мигания= 2с (2000мс)
  TIM5::SR::UIF::NoUpdate::Set(); //сбрасываем флаг запроса на прерывание
  TIM5::CNT::Write(); //запись нуля в счетчик
  TIM5::CR1::CEN::Enable::Set(); // включение таймера
  TIM5::DIER::UIE::Enable::Set(); //включение прерываний
  
  
  return 1;
}
}


int main()
{

  for(;;)
  {
  }
  
  return 0 ;
}

