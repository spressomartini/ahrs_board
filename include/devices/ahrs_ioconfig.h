#include "stm32f3hal/gpio.h"

#define GREEN_LED_PIN   (0u)
#define RED_LED_PIN     (1u)
#define BMX055_POW_PIN  (5u)

/* ==================
 *  UART GPIO CONFIG
 * ==================*/
extern GPIO_PIN_Config usart1tx_pin_config;
extern GPIO_PIN_Config usart1rx_pin_config;

/* =================
 *  LED GPIO CONFIG
 * =================*/
extern GPIO_PIN_Config green_led_config;
extern GPIO_PIN_Config red_led_config;

/* ====================
 *  BMX055 GPIO CONFIG
 * ====================*/
extern GPIO_PIN_Config bmx055_pow_config;
extern GPIO_PIN_Config i2c1_scl_pin_config;
extern GPIO_PIN_Config i2c1_sda_pin_config;
