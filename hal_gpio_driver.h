#ifndef _HAL_GPIO_DRIVER_H
#define _HAL_GPIO_DRIVER_H

#include "stm32f407xx.h"

//-----------------------------------------------------------------------------
//              Macros used for GPIO pin initialization
//              (as listed in the reference manual)
//-----------------------------------------------------------------------------

// GPIO mode settings values
#define GPIO_PIN_INPUT_MODE             ( (uint32_t)0x00 )
#define GPIO_PIN_OUTPUT_MODE            ( (uint32_t)0x01 )
#define GPIO_PIN_ALT_FUN_MODE           ( (uint32_t)0x02 )

// GPIO output type selection values
#define GPIO_PIN_OP_TYPE_PUSHPULL       ( (uint32_t)0x00 )
#define GPIO_PIN_OP_TYPE_OPEN_DRAIN     ( (uint32_t)0x01 )

// GPIO speed type selection values
#define GPIO_PIN_SPEED_LOW              ( (uint32_t)0x00 )
#define GPIO_PIN_SPEED_MEDIUM           ( (uint32_t)0x01 )
#define GPIO_PIN_SPEED_HIGH             ( (uint32_t)0x02 )
#define GPIO_PIN_SPEED_VERY_HIGH        ( (uint32_t)0x03 )

// GPIO pull up/pull down selection values
#define GPIO_PIN_NO_PULL_PUSH           ( (uint32_t)0x00 )
#define GPIO_PIN_PULL_UP                ( (uint32_t)0x01 )
#define GPIO_PIN_PULL_DOWN              ( (uint32_t)0x11 )

// GPIO port address definitions
#define GPIO_PORT_A                     GPIOA
#define GPIO_PORT_B                     GPIOB
#define GPIO_PORT_C                     GPIOC
#define GPIO_PORT_D                     GPIOD
#define GPIO_PORT_E                     GPIOE
#define GPIO_PORT_F                     GPIOF
#define GPIO_PORT_G                     GPIOG
#define GPIO_PORT_H                     GPIOH
#define GPIO_PORT_I                     GPIOI

// Macros to enable clock for different GPIO ports in RCC register:
// The registers of Reset and Clock Control (RCC) block are used to
// enable/disable the clocks for various periferals including GPIO
#define _HAL_RCC_GPIOA_CLK_ENABLE()     (RCC->AHB1ENR |= (1 << 0) )
#define _HAL_RCC_GPIOB_CLK_ENABLE()     (RCC->AHB1ENR |= (1 << 1) )
#define _HAL_RCC_GPIOC_CLK_ENABLE()     (RCC->AHB1ENR |= (1 << 2) )
#define _HAL_RCC_GPIOD_CLK_ENABLE()     (RCC->AHB1ENR |= (1 << 3) )
#define _HAL_RCC_GPIOE_CLK_ENABLE()     (RCC->AHB1ENR |= (1 << 4) )
#define _HAL_RCC_GPIOF_CLK_ENABLE()     (RCC->AHB1ENR |= (1 << 5) )
#define _HAL_RCC_GPIOG_CLK_ENABLE()     (RCC->AHB1ENR |= (1 << 6) )
#define _HAL_RCC_GPIOH_CLK_ENABLE()     (RCC->AHB1ENR |= (1 << 7) )

//-----------------------------------------------------------------------------
//              Data structure for GPIO pin initialization
//              (used by the app to initialize the hardware)
//-----------------------------------------------------------------------------

typedef struct
{
    uint32_t pin;           // Specifies the GPIO pins to be configured
    uint32_t mode;          // Specifies the operating mode for the selected pins
    uint32_t op_type;       // Specifies the output type for the selected pins
    uint32_t pull;          // Specifies the pull-up or pull-down activation for the selected pins
    uint32_t speed;         // Specifies the speed for the selected pins
    uint32_t alternate;     // Specifies the alternate function value if the mode is set for alt func mode
} gpio_pin_config_t;

// I2C pinspack enumeration
typedef enum
{
    INT_RISING_EDGE,
    INT_FALLING_EDGE,
    INT_RISING_FALLING_EDGE
} int_edge_sel_t;

//-----------------------------------------------------------------------------
//              Driver exposed APIs
//              
//-----------------------------------------------------------------------------

// Initializes the GPIO pin
//  GPIOx: GPIO port base address
//  gpio_pin_config: pointer to the pin config structure sent by the application
void hal_gpio_init(GPIO_TypeDef* GPIOx, gpio_pin_config_t* gpio_pin_config);

// Read a value from a given GPIO pin
//  GPIOx: GPIO port base address
//  pin_no: GPIO pin number
//  uint8_t: value read
uint8_t hal_gpio_read_from_pin(GPIO_TypeDef* GPIOx, uint16_t pin_no);

// Write a value to a given GPIO pin
//  GPIOx: GPIO port base address
//  pin_no: GPIO pin number
//  value: value to be written
void hal_gpio_write_to_pin(GPIO_TypeDef* GPIOx, uint16_t pin_no, uint8_t value);

// Set the alternate functionality for a given GPIO pin
//  GPIOx: GPIO port base address
//  pin_no: GPIO pin number
//  alt_func_value: altertate function value for which to configure the pin
void hal_gpio_set_alt_function(GPIO_TypeDef* GPIOx, uint16_t pin_no, uint16_t alt_func_value);

#endif
