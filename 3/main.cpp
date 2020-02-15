#include "gpioaregisters.hpp"
#include "gpiocregisters.hpp" //
#include "rccregisters.hpp"
#include "tim2registers.hpp"


extern "C" //связывание с СИ?
{
  int __low_level_init(void) //низкоуровневый логический сигнал
  {
    RCC::CR::HSION::On::Set(); //включаем внутренний генератор частоты
    while ( !RCC::CR::HSIRDY::Enable::IsSet( ) ) // ждем, пока не установится нужный режим тактирования
     {
       
     };
    
    RCC::CFGR::SW::Hsi::Set(); // используем внутренний генератор для источника тактирования
    while ( !RCC::CFGR::SWS::Hsi::IsSet( ) ) //снова ждем, пока не установится нужный режим
    {
      
    };
    
    RCC::AHB1ENR::GPIOCEN::Enable::Set() ; //подключение к системе тактирования джпиоа
    RCC::AHB1ENR::GPIOAEN::Enable::Set() ; //подключение к системе тактирования джпиоц
    GPIOA::MODER::MODER5::Output::Set() ; //настраиваем порт А5 на выход
    GPIOC::MODER::MODER5::Output::Set() ; //настраиваем порт С5 на выход
    GPIOC::MODER::MODER8::Output::Set() ; //настраиваем порт С8 на выход
    GPIOC::MODER::MODER9::Output::Set() ; //настраиваем порт С9 на выход
    GPIOC::MODER::MODER13::Input::Set() ; //настраиваем порт С13 на вход
    
    RCC::APB1ENR::TIM2EN::Enable::Set() ; //включаем таймер2 к тактированию
    TIM2::CR1::URS::OverFlow::Set(); //утанавливаем прерывание по переполнению
    return 0;
  }
}

void Delay (std::uint32_t delay) //процедура задержки

{
  TIM2::ARR::Write(16'000 * delay ); //значение до которого будет считать счетчик
  TIM2::SR::UIF::NoUpdate::Set(); //скидываем флаг генерации прерывания
  TIM2::CNT::Write(0); //устанавливаем начальное значение счетчика в 0
  TIM2::CR1::CEN::Enable::Set(); //запускаем счетчик
  while (TIM2::SR::UIF::NoUpdate::IsSet() ) //проверяем пока не установлен флаг генерации прерывания по событию
  {
    
  }
  TIM2::CR1::CEN::Disable::Set(); //останавливаем счетчик
}

template <typename Port, std::uint32_t pinNum> //шаблон с типом Порт
struct Led //процедура переключения светодиода
{
  static void Toggle() //процедура включение/выключение
  {
    Port::ODR::Toggle(1 << pinNum); //изменяем значение номера пина
  }
};

using Led1 = Led<GPIOC, 5> ; //меняется режим лампы 1
using Led2 = Led<GPIOC, 8> ; //меняется режим лампы 2
using Led3 = Led<GPIOC, 9> ; //меняется режим лампы 3
using Led4 = Led<GPIOA, 5> ; //меняется режим лампы 4

int main()
{
  for(;;)
  {
    Led1::Toggle(); //лампочка 1 включена
    Led2::Toggle(); //лампочка 2 включена
    Led3::Toggle(); //лампочка 3 включена
    Led4::Toggle(); //лампочка 4 включена
    Delay(500); //задержка 0,5 секунды
    Led1::Toggle(); //лампочка 1 выключена
    Led2::Toggle();//лампочка 2 выключена
    Led3::Toggle();//лампочка 3 выключена
    Led4::Toggle();//лампочка 4 выключена
    Delay(500); //задержка 0,5 секунды
    Led1::Toggle(); //лампочка 1 включена
    Led2::Toggle(); //лампочка 2 включена
    Led3::Toggle(); //лампочка 3 включена
    Led4::Toggle(); //лампочка 4 включена
    Delay(1'000); //задержка 1 секунда
    Led1::Toggle(); //лампочка 1 выключена
    Led2::Toggle(); //лампочка 2 выключена
    Led3::Toggle(); //лампочка 3 выключена
    Led4::Toggle(); //лампочка 4 выключена
    Delay(1'000); //задержка 1 секунда
    Led1::Toggle(); //лампочка 1 включена
    Led2::Toggle(); //лампочка 2 включена
    Led3::Toggle(); //лампочка 3 включена
    Led4::Toggle(); //лампочка 4 включена
    Delay(1'500); //задержка 1,5 секунды
    Led1::Toggle(); //лампочка 1 выключена
    Led2::Toggle(); //лампочка 2 выключена
    Led3::Toggle(); //лампочка 3 выключена
    Led4::Toggle(); //лампочка 4 выключена
    Delay(1'500); //задержка 1,5 секунды
  }
  return 0;
}
