
#pragma once

#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "picolib/picolib.hpp"

#include <gcisensors.hpp>

// #define MSG_MAVLINK 1
// #define MSG_YIVO !MSG_MAVLINK

constexpr uint32_t LED_PIN            = 25;
constexpr uint32_t ADC_BATT_PIN       = A2;

constexpr uint32_t WATCHDOG_RESET = 4000; // up to 8.3 sec

constexpr uint32_t PWM_M0             = 20;
constexpr uint32_t PWM_M1             = 21;
constexpr uint32_t PWM_M2             = 22;
constexpr uint32_t PWM_M3             = 23;

constexpr uint32_t I2C_PORT           = 0;
constexpr uint32_t i2c_scl            = I2C0_SCL_PIN;
constexpr uint32_t i2c_sda            = I2C0_SDA_PIN;

// INFO ////////////////////////////////////////////////////////////
bi_decl(bi_program_description("something"));
bi_decl(bi_program_version_string("v1"));
bi_decl(bi_program_url("github"));
// I2C ------------------------------------------------------------
bi_decl(bi_2pins_with_func(i2c_sda, i2c_scl, GPIO_FUNC_I2C));
// UART -----------------------------------------------------------
bi_decl(bi_2pins_with_func(UART0_RX_PIN, UART0_TX_PIN, GPIO_FUNC_UART));
bi_decl(bi_2pins_with_func(UART1_RX_PIN, UART1_TX_PIN, GPIO_FUNC_UART));
// ADC ------------------------------------------------------------
bi_decl(bi_1pin_with_name(ADC_BATT_PIN, "Battery ADC"));
// PWM ------------------------------------------------------------
bi_decl(bi_1pin_with_name(PWM_M0, "PWM M0"));
bi_decl(bi_1pin_with_name(PWM_M1, "PWM M1"));
bi_decl(bi_1pin_with_name(PWM_M2, "PWM M2"));
bi_decl(bi_1pin_with_name(PWM_M3, "PWM M3"));
// LED ------------------------------------------------------------
bi_decl(bi_1pin_with_name(LED_PIN, "LED"));
////////////////////////////////////////////////////////////////////