//
// Created by SKolody on 17.10.2019.
//

#ifndef REGISTERS_TIMER_HPP
#define REGISTERS_TIMER_HPP

#include "susudefs.hpp"   //for __forceinline
#include "subscriber.hpp" //for ISubscriber

struct TimerSwitchable
{
};

struct TimerCountable: TimerSwitchable
{
};

struct TimerCcpable: TimerSwitchable
{
};


struct TimerInterruptable
{
};

struct TimerCountableInterruptable: TimerInterruptable, TimerCountable
{
};

struct TimerCcpableInterruptable: TimerInterruptable, TimerCcpable
{
};


template<typename TimerModule, typename Interface, auto* const ...args>
struct Timer
{
  using CntType = typename TimerModule::CNT::Type ;
  
  __forceinline template<typename T = Interface,
          class = typename std::enable_if_t<std::is_base_of<TimerSwitchable, T>::value>>
  static void Start()
  {
    TimerModule::SR::UIF::NoUpdate::Set();
    TimerModule::CNT::Write(CntType(0));
    TimerModule::CR1::CEN::Enable::Set() ;
  }
  
  __forceinline template<typename T = Interface,
          class = typename std::enable_if_t<std::is_base_of<TimerSwitchable, T>::value>>
  static void Stop()
  {
    TimerModule::CR1::CEN::Disable::Set() ;
    TimerModule::SR::UIF::NoUpdate::Set() ;
  }
  
  __forceinline template<typename T = Interface,
          class = typename std::enable_if_t<std::is_base_of<TimerCountable, T>::value>>
  static void SetDelay(uint32_t delay)
  {
    TimerModule::ARR::Write(delay) ;
    Start();
    while(!TimerModule::SR::UIF::UpdatePending::IsSet())
    {
    }
    Stop();
  }
  
  __forceinline template<typename T = Interface,
          class = typename std::enable_if_t<std::is_base_of<TimerInterruptable, T>::value>>
  void InterruptHandle() const
  {
    auto subscribers = {(ISubscriber*)(args)...} ;  
    for (auto subscriber: subscribers)
    {
      subscriber->Update() ;
    }
  }
};

#endif //REGISTERS_TIMER_HPP