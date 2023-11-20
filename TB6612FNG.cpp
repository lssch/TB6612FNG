//
// Created by Lars Schwarz on 24.10.2023.
//

#include "TB6612FNG.h"
#include <cmath>

void Channel::off() {
  current_speed = 0;
  set_speed();
  brake();
}

void Channel::move(int8_t speed) {
  current_speed = speed;
  set_speed();
}

void Channel::brake() {
  HAL_GPIO_WritePin(port.gpio_port_1, port.gpio_pin_1, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(port.gpio_port_2, port.gpio_pin_2, GPIO_PIN_RESET);
}

void Channel::set_speed() {

  if (current_speed > 0) {
    // Drive CW
    HAL_GPIO_WritePin(port.gpio_port_1, port.gpio_pin_1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(port.gpio_port_2, port.gpio_pin_2, GPIO_PIN_RESET);
  } else {
    // Drive CCW
    HAL_GPIO_WritePin(port.gpio_port_1, port.gpio_pin_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(port.gpio_port_2, port.gpio_pin_2, GPIO_PIN_SET);
  }

  // set pwm according to the current speed

  float scale = static_cast<float>(abs(current_speed))/100;

  port.htim->Instance->CCR1 = scale*port.htim->Instance->ARR;
  HAL_TIM_PWM_Start(port.htim, port.channel);
}

void Channel::set_port(channel_port_t port_) {
  port = port_;
}

TB6612FNG::TB6612FNG(GPIO_TypeDef* gpio_port_enb, uint16_t gpio_pin_enb,
                     GPIO_TypeDef* gpio_port_i1, uint16_t gpio_pin_i1,
                     GPIO_TypeDef* gpio_port_i2, uint16_t gpio_pin_i2,
                     TIM_HandleTypeDef *timer_, uint16_t channel_):
                     gpio_port_enable(gpio_port_enb),
                     gpio_pin_enable(gpio_pin_enb) {
  channel_port_t port_a = {
    .gpio_port_1 = gpio_port_i1,
    .gpio_pin_1 = gpio_pin_i1,
    .gpio_port_2 = gpio_port_i2,
    .gpio_pin_2 = gpio_pin_i2,
    .htim = timer_,
    .channel = channel_
  };
  /*
  channel_port_t port_a = {
          .gpio_port_1 = gpio_port_bi1,
          .gpio_pin_1 = gpio_pin_bi1,
          .gpio_port_2 = gpio_port_bi2,
          .gpio_pin_2 = gpio_pin_bi2,
          .htim = timer_b,
          .channel = channel_b
  };
  */
  ch_a.set_port(port_a);
  ch_b.set_port(port_a);
}

void TB6612FNG::enable() {
  HAL_GPIO_WritePin(gpio_port_enable, gpio_pin_enable, GPIO_PIN_SET);
}

void TB6612FNG::disable() {
  HAL_GPIO_WritePin(gpio_port_enable, gpio_pin_enable, GPIO_PIN_RESET);
}
