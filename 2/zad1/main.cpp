#include <cstdint>
#include <cstddef>
#include <iostream>
#include "rccregisters.hpp"
#include "gpiocregisters.hpp"
using namespace std;

//константные значения, которые не будут меняться:

constexpr size_t GpiocAddr = 0x40020800; //адрес регистра gpioc
constexpr size_t ModerOffset = 0x00; //смещение адреса регистра модер относительно джпиоц (стр. 157)
constexpr size_t OdrOffset = 0x14; //смещение адреса регистра одр относительно джпиоц (стр 159.)
constexpr size_t ModerAddr = GpiocAddr + ModerOffset; //записываем адрес регистра модер
constexpr size_t OdrAddr = GpiocAddr + OdrOffset; //записываем адрес регистра одр
using namespace std;
void delay(uint32_t delay) //процедура задержки
 {
   for (int i=0; i<delay; i++)
   {
   }
 }

int main() 
{
RCC::AHB1ENR::GPIOCEN::Enable::Set() ; //включаем тактирование портов джпиоц

  volatile uint32_t *ModerPointer = reinterpret_cast<volatile uint32_t*>(ModerAddr); //преобразует модер в модерпойнтер
  volatile uint32_t *OdrPointer = reinterpret_cast<volatile uint32_t*>(OdrAddr ) ; //преобразует одр в одрпойнтер
  for(;;) 
   {
      *ModerPointer |= ((1 << 10 )|(1<<16)|(1<<18)); //выставляем 10, 16 и 18 биты регистра модер в 1
      *ModerPointer &= ~ ((1 << 11 )|(1<<17)|(1<<19)); //выставляем 11, 17 и 19 биты регистра модер в 0
      *OdrPointer |= ((1 << 5 )|(1<<8)|(1<<9)); //выставляем 5, 8 и 9 биты регистра одр в 1
      delay (1'000'000); //задержка
      *OdrPointer &= ~ ((1 << 5 )|(1<<8)|(1<<9)); //выставляем 5, 8 и 9 биты регистра одр в 0
   }
  return 0 ;
}
