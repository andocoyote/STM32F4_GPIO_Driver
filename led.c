#include "led.h"

void led_init(void)
{
    gpio_pin_config_t led_pin_config;
    
    // Enable the clock for GPIO port GPIOD
    _HAL_RCC_GPIOD_CLK_ENABLE();
    
    led_pin_config.pin = LED_ORANGE;
    led_pin_config.mode = GPIO_PIN_OUTPUT_MODE;
    led_pin_config.op_type = GPIO_PIN_OP_TYPE_PUSHPULL;
    led_pin_config.speed = GPIO_PIN_SPEED_MEDIUM;
    led_pin_config.pull = GPIO_PIN_NO_PULL_PUSH; // Don't need an internal resistor
    
    // Configure all of the pins to which an LED is attached on the board (GPIOD 12-15)
    hal_gpio_init(GPIO_PORT_D, &led_pin_config);
    
    led_pin_config.pin = LED_BLUE;
    hal_gpio_init(GPIO_PORT_D, &led_pin_config);
    
    led_pin_config.pin = LED_GREEN;
    hal_gpio_init(GPIO_PORT_D, &led_pin_config);
    
    led_pin_config.pin = LED_RED;
    hal_gpio_init(GPIO_PORT_D, &led_pin_config);
}

void led_turn_on(GPIO_TypeDef *GPIOx, uint16_t pin)
{
    if(GPIOx)
    {
        hal_gpio_write_to_pin(GPIOx, pin, 1);
    }
}

void led_turn_off(GPIO_TypeDef *GPIOx, uint16_t pin)
{
    if(GPIOx)
    {
        hal_gpio_write_to_pin(GPIOx, pin, 0);
    }
}

void led_toggle(GPIO_TypeDef *GPIOx, uint16_t pin)
{
    if(GPIOx)
    {
        if(hal_gpio_read_from_pin(GPIOx, pin))
        {
            hal_gpio_write_to_pin(GPIOx, pin, 0);
        }
        else
        {
            hal_gpio_write_to_pin(GPIOx, pin, 0);
        }
    }
}

int main(void)
{
    uint32_t i = 0;
    
    // Initialize the LEDs
    led_init();
    
    while(1)
    {
        led_turn_on(GPIO_PORT_D, LED_ORANGE);
        led_turn_on(GPIO_PORT_D, LED_BLUE);
        
        for(i = 0; i < 500000; i++);
        
        led_turn_off(GPIO_PORT_D, LED_ORANGE);
        led_turn_off(GPIO_PORT_D, LED_BLUE);
        
        for(i = 0; i < 500000; i++);
    }
}
