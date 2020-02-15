#include <cstdint>
#include <cstddef>
#include <iostream>
#include "rccregisters.hpp"
#include "gpiocregisters.hpp"
using namespace std;
constexpr size_t GpiocAddr = 0x40020800; //создадим константный цельночисленное беззнаковый тип GpiocAddr по которому запишем адрес GPIOC
constexpr size_t ModerOffset = 0x00; //смещение MODER
constexpr size_t OdrOffset = 0x14; //смещение ODR
constexpr size_t ModerAddr = GpiocAddr + ModerOffset; //адрес регистра GPIOC_MODER
constexpr size_t OdrAddr = GpiocAddr + OdrOffset; //адрес регистра GPIOC_ODR

using namespace std;
void delay(uint32_t delay)
 {
   for (int i=0; i<delay; i++)
   {
   }
 }

int main() 
{
RCC::AHB1ENR::GPIOCEN::Enable::Set() ;

  volatile uint32_t *ModerPointer = reinterpret_cast<volatile uint32_t*>(ModerAddr); //создадим указатель ModerPointer, который может меняться из вне, 32-битовый, без знака, преобразовывая ModerAddr в указатель типа volatile uint32_t
  volatile uint32_t *OdrPointer = reinterpret_cast<volatile uint32_t*>(OdrAddr ) ; //указатель на адрес GPIOC_ODR
  for(;;) 
   {
      *ModerPointer |= ((1 << 10 )|(1<<16)|(1<<18));   //Запишем по адресу регистра GPIOC_MODER биты 10,16,18 в 1
      *ModerPointer &= ~ ((1 << 11 )|(1<<17)|(1<<19)); //Запишем по адресу регистра GPIOC_MODER биты 19,17,11 в 0
      *OdrPointer |= ((1 << 5 )|(1<<8)|(1<<9)); //Запишем по адресу регистра GPIOC_ODR биты 5,8,9 в 1
      delay (1'000'000);
      *OdrPointer &= ~ ((1 << 5 )|(1<<8)|(1<<9)); //Запишем по адресу регистра GPIOC_ODR биты 5,8,9 в 0  
   }
  return 0 ;
}

//0x4002 0800
//     uint32_t Moder = *ModerPointer; //запишем в переменную Moder адрес, разыменовывая указатель ModerPointer  
//     cout << Moder << endl ; //выведим значение переменной Moder
//    *ModerPointer |= ((1 << 10 )|(1<<16)|(1<<18));   
 //   *ModerPointer &= ~ ((1 << 11 )|(1<<17)|(1<<19));
 //   *OdrPointer |= ((1 << 5 )|(1<<8)|(1<<9));
  //  *OdrPointer &= ~ ((1 << 5 )|(1<<8)|(1<<9));