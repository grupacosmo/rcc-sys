//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef RCC_SYS_UARTBUS_H
#define RCC_SYS_UARTBUS_H

#include <Bus.h>

namespace hw {

class UARTBus : public Bus<UARTHandle> {
 public:
  constexpr explicit UARTBus(UARTHandle* handle) : Bus(handle) {}
  inline auto transmit(uint8_t* data, uint16_t size, uint32_t timeout = defaultTimeout) const
      -> BusResult;

 private:
  static constexpr uint32_t defaultTimeout = 100;
};

auto UARTBus::transmit(uint8_t* data, uint16_t size, uint32_t timeout) const -> BusResult {
  return static_cast<BusResult>(HAL_UART_Transmit(handle_, data, size, timeout));
}

}  // namespace hw

#endif  // RCC_SYS_UARTBUS_H