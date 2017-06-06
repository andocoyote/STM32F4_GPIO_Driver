#include "led.h"

/* led_init
    1. Enable the clock for GPIOD so it can do work
    2. For each led pin (12, 13, 14, 15):
        a. Configure the pin for output
        b. Configure the pin for push/pull (not open drain, i.e. float/HiZ)
        c. Configure the pin for medium speed clock
        d. Configure the pin to not use the internal resistor for floating
*/
void led_init(void)
{
    gpio_pin_config_t led_pin_config;
    
    // Enable the clock for GPIO port GPIOD
    _HAL_RCC_GPIOD_CLK_ENABLE();
    
    led_pin_config.pin = LED_ORANGE;
    led_pin_config.mode = GPIO_PIN_OUTPUT_MODE;         // Configure for output
    led_pin_config.op_type = GPIO_PIN_OP_TYPE_PUSHPULL; // Never float/HiZ
    led_pin_config.speed = GPIO_PIN_SPEED_MEDIUM;       // Medium speed
    led_pin_config.pull = GPIO_PIN_NO_PULL;             // Don't need an internal resistor
    
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
            hal_gpio_write_to_pin(GPIOx, pin, 1);
        }
    }
}

int main(void)
{
    // uint32_t i = 0; // This is used by the first exercise.  Commenting out
    
    // Initialize the GPIO pins for the LEDs (output, speed, push/pull, no internal resistors)
    // Also enables the clock for GPIOD which contains the LED pins
    led_init();
    
    // Enable the clock for the GPOIA port (button is on GPIOA, pin 0)
    _HAL_RCC_GPIOA_CLK_ENABLE();
    
    // Configure the button interrupt as falling edge
    hal_gpio_configure_interrupt(GPIO_BUTTON_PIN, INT_FALLING_EDGE);
    
    // Enable the interrupt on EXTI0 line
    hal_gpio_enable_interrupt(GPIO_BUTTON_PIN, EXTI0_IRQn /*IRQ 6*/);
    

#if 0 // This effectively comments out the code from the first exercise
    while(1)
    {
        led_turn_on(GPIO_PORT_D, LED_ORANGE);
        led_turn_on(GPIO_PORT_D, LED_BLUE);
        
        for(i = 0; i < 500000; i++);
        
        led_turn_off(GPIO_PORT_D, LED_ORANGE);
        led_turn_off(GPIO_PORT_D, LED_BLUE);
        
        for(i = 0; i < 500000; i++);
    }
#endif
    
    // Run in an infinite loop.  The EXTI0_IRQHandler is firing every time
    // the blue button is pressed.
    while(1);
}

// ISR for the configured EXTI0 interrupt
void EXTI0_IRQHandler(void)
{
    // Clear the interrupt so it doesn't fire again
    // Else, it would keep firing and this handler would keep getting called
    hal_gpio_clear_interrupt(GPIO_BUTTON_PIN);
    
    // Execute the ISR work
    led_toggle(GPIOD, LED_BLUE);
    led_toggle(GPIOD, LED_ORANGE);
    led_toggle(GPIOD, LED_RED);
    led_toggle(GPIOD, LED_GREEN);
}
