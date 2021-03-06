//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef RCC_SYS_GPIOPIN_H
#define RCC_SYS_GPIOPIN_H

#include "Noncopyable.h"
#include "stm32f4xx.h"

namespace hw {

enum GPIOState { Reset, Set };

class GPIOPin : public Noncopyable {
 public:
  GPIOPin(GPIO_TypeDef* pinType, uint16_t pinNumber);
  void set();
  void reset();
  void toggle();
  [[nodiscard]] auto read() const -> GPIOState;

 private:
  GPIO_TypeDef* const pinType_;
  const uint16_t pinNumber_;
};

}  // namespace hw

#endif  // RCC_SYS_GPIOPIN_H
