
#ifndef REGISTERS_INTERRUPTHANDLER_HPP
#define REGISTERS_INTERRUPTHANDLER_HPP


#include "gpioaregisters.hpp"  //for Gpioa
#include "gpiocregisters.hpp"  //for Gpioc
#include "gpiobregisters.hpp"  //for Gpiob
#include "rccregisters.hpp"    //for RCC
#include "tim2registers.hpp"   //for SPI2
#include "nvicregisters.hpp"   //for NVIC
#include "usart2registers.hpp" //for USART2

class InterruptHandler {
  public:
    static void DummyHandler() { for(;;) {} }
    static void USART2Handler()
    {
      char str[] = "Hello World! ";
      char *ptr; 
      ptr = &str[0];
      for(;;)    
      {
         USART2::DR::Write(*ptr) ;  
         while(!USART2::SR::TXE::DataRegisterEmpty::IsSet()) ;
         ptr ++ ;
         if (*ptr == 0)
            {
            ptr = str ;
            }
        while(TIM2::SR::UIF::NoUpdate::IsSet()) ; 
              TIM2::SR::UIF::NoUpdate::Set();
        }
    }
};

#endif //REGISTERS_INTERRUPTHANDLER_HPP//50

