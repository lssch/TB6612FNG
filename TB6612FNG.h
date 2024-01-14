//
// Created by Lars Schwarz on 24.10.2023.
//

#ifndef STM32_MOTORDRIVER_TB6612FNG_H
#define STM32_MOTORDRIVER_TB6612FNG_H

#include "stm32f4xx_hal.h"

typedef struct _channel_port_t{
  GPIO_TypeDef* gpio_port_1;
  uint16_t gpio_pin_1;
  GPIO_TypeDef* gpio_port_2;
  uint16_t gpio_pin_2;
  TIM_HandleTypeDef* htim;
  uint16_t channel;
} channel_port_t;

class Channel{
public:
  /// @brief Assign the hardware gpio and timer to the channel.
  /// @param[in] port_ Parameter to describe the port.
  void set_port(const channel_port_t port_);

  /// @brief Switch of the channel
  void off();

  /// @brief Set the speed of the channel
  /// @param[in] speed Speed in percent.
  void move(const int8_t speed);

  /// @brief Active breaking by using both outputs.
  void brake();
private:
  channel_port_t port;
  int8_t current_speed;

  void set_speed();
};

class TB6612FNG{
public:
  Channel ch_a;
  Channel ch_b;

  /// @brief Motordriver object for the TB6612FNG IC
  /// @param[in,out] gpio_port_enb Output to enable the IC
  /// @param[in,out] gpio_pin_enb Output to enable the IC
  /// @param[in,out] gpio_port_m1_i1 Output for cw channel 1
  /// @param[in,out] gpio_pin_m1_i1 Output for cw channel 1
  /// @param[in,out] gpio_port_m1_i2 Output for ccw channel 1
  /// @param[in,out] gpio_pin_m1_i2 Output for ccw channel 1
  /// @param[in,out] gpio_port_m2_i1 Output for cw channel 2
  /// @param[in,out] gpio_pin_m2_i1 Output for cw channel 2
  /// @param[in,out] gpio_port_m2_i2 Output for ccw channel 2
  /// @param[in,out] gpio_pin_m2_i2 Output for ccw channel 2
  /// @param[in,out] timer_m1_ Timer for channel 1
  /// @param[in] channel_m1_ Timer for channel 1
  /// @param[in,out] timer_m2_ Timer for channel 2
  /// @param[in] channel_m2_ Timer for channel 2
  /// @todo Automatically detect if the chip needs to be enabled or disabled based on the two channels.
  TB6612FNG(GPIO_TypeDef* gpio_port_enb, uint16_t gpio_pin_enb,
            GPIO_TypeDef* gpio_port_m1_i1, uint16_t gpio_pin_m1_i1,
            GPIO_TypeDef* gpio_port_m1_i2, uint16_t gpio_pin_m1_i2,
            GPIO_TypeDef* gpio_port_m2_i1, uint16_t gpio_pin_m2_i1,
            GPIO_TypeDef* gpio_port_m2_i2, uint16_t gpio_pin_m2_i2,
            TIM_HandleTypeDef *timer_m1_, uint16_t channel_m1_, TIM_HandleTypeDef *timer_m2_, uint16_t channel_m2_);

  /// @brief Enable the IC
  void enable();

  /// @brief Disable the IC
  void disable();
private:
  GPIO_TypeDef* gpio_port_enable;
  uint16_t gpio_pin_enable;
  bool is_enabled = false;
};


#endif //STM32_MOTORDRIVER_TB6612FNG_H
