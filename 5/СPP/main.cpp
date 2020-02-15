//#include <cstdint>             //for int types such as uint32_t
//#include <stdio.h>
#include "gpioaregisters.hpp"  //for Gpioa
#include "gpiocregisters.hpp"  //for Gpioc
#include "gpiobregisters.hpp"  //for Gpiob
#include "rccregisters.hpp"    //for RCC
#include "tim2registers.hpp"   //for SPI2
#include "nvicregisters.hpp"   //for NVIC
#include "usart2registers.hpp" //for USART2

using namespace std ;

extern "C"
{
int __low_level_init(void)
  {
  //частота
  RCC::CR::HSEON::On::Set(); //подключение внешней частоты
  while(!RCC::CR::HSERDY::Ready::IsSet()) //ожидание стабилизация источника
  {};
  RCC::CFGR::SW::Hse::Set(); //назначаем источник на системную частоту
  while(!RCC::CFGR::SWS::Hse::IsSet()) //ожидание переключения источника на системную частоту
  {};
  //порты
  RCC::AHB1ENR::GPIOAEN::Enable::Set(); //включение тактирования портов А
  RCC::AHB1ENR::GPIOCEN::Enable::Set(); //включение тактирования портов С
  //таймер
  RCC::APB1ENR::TIM2EN::Enable::Set(); //включение 2 таймера 
  TIM2::PSC::Write(7999); //деление на 8000
  TIM2::ARR::Write(38); //скорость 0,5 мс (В "Hello World! " 13 символов. 500/13=38)
  TIM2::CR1::CEN::Enable::Set(); //включение счетчика
  //USART2
  RCC::APB1ENR::USART2EN::Enable::Set(); //Подключение модуля USART2 к шине тактирования
  GPIOA::MODER::MODER2::Alternate::Set(); //Альтернативный режим порта А.2
  GPIOA::MODER::MODER3::Alternate::Set(); //Альтернативный режим порта А.3
  GPIOA::OTYPER::OT2::OutputPushPull::Set(); // настройка типа вывода: двухтактный выход (Output push-pull) порта А.2
  GPIOA::OTYPER::OT3::OutputPushPull::Set(); // настройка типа вывода: двухтактный выход (Output push-pull) порта А.3
  GPIOA::PUPDR::PUPDR2::PullUp::Set(); //pull-up порта А.2 (подтяжка к 1)
  GPIOA::PUPDR::PUPDR3::PullUp::Set(); //pull-up порта А.3 (подтяжка к 1)
  GPIOA::AFRL::AFRL2::Af7::Set(); //биты для настройки альтернативных функций ввода-вывода. порт А.2
  GPIOA::AFRL::AFRL3::Af7::Set(); //биты для настройки альтернативных функций ввода-вывода. порт А.3
  USART2::BRR::Write(52<<4); //Регистр настройки скорости передачи. Счетчики бода перестают считать, если биты TE или RE отключены соответственно
  //USARTDIV = CLK/(BaudRate*8*(2 - OVER8))​
  //USARTDIV = 8000/(19200*8*(2-1((OVER8=1)режим дискретизации 1/8)))= 52
  USART2::CR2::STOP::Value2::Set();//2 стоп бита
  NVIC::ISER1::Write(1<<6); //разрешить глобальное прерывание по USART​2?
  //(страница 203?) за включение прерываний 32-63  отвечает ISER[1]. наше прерывание - 38. 38-31=6. 
    
  USART2::CR1::UE::Enable::Set(); //Включение USART2
  USART2::CR1::TE::Enable::Set(); //Включение передачи
  USART2::CR1::RE::Enable::Set(); //Включение приема
  USART2::CR1::TXEIE::InterruptWhenTXE::Set();  //разрешить прерывание по передаче​
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