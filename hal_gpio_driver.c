#include "hal_gpio_driver.h"

//-----------------------------------------------------------------------------
//              Static helper functions
//              
//-----------------------------------------------------------------------------

// Configure the pin mode for a given GPIO pin
//  GPIOx: GPIO port base address
//  pin_no: GPIO pin number
//  mode: mode to be configured
static void hal_gpio_configure_pin_mode(GPIO_TypeDef* GPIOx, uint16_t pin_no, uint32_t mode)
{
    if(GPIOx)
    {
        // Set the mode register (MODER)
        // Since there are 2 configuration bits per bin, multiply the pin number
        // by 2 to get the actual bits we're supposed to set
        GPIOx->MODER |= (mode << (2 * pin_no));
    }
}

// Configure the pin output type for a given GPIO pin
//  GPIOx: GPIO port base address
//  pin_no: GPIO pin number
//  op_type: output type for which to configure the pin
static void hal_gpio_configure_pin_output_type(GPIO_TypeDef* GPIOx, uint16_t pin_no, uint32_t op_type)
{
    // 0 = output pushpull
    // 1 = output open drain
    if(GPIOx)
    {
        // Set the output type register (OTYPER)
        GPIOx->OTYPER |= (op_type << pin_no);
    }
}

// Configure the speed for a given GPIO pin
//  GPIOx: GPIO port base address
//  pin_no: GPIO pin number
//  speed: the speed to configure the pin with
static void hal_gpio_configure_pin_speed(GPIO_TypeDef* GPIOx, uint16_t pin_no, uint32_t speed)
{
    if(GPIOx)
    {
        // Set the output speed register (OSPEEDR)
        // Since there are 2 configuration bits per bin, multiply the pin number
        // by 2 to get the actual bits we're supposed to set
        GPIOx->OSPEEDR |= (speed << (2 * pin_no));
    }
}

// Activate the internal pull up or pull down resistors
//  GPIOx: GPIO port base address
//  pin_no: GPIO pin number
//  pupd: specifies the register to activate
static void hal_gpio_configure_pin_pupd(GPIO_TypeDef* GPIOx, uint16_t pin_no, uint32_t pupd)
{
    if(GPIOx)
    {
        // Enable or disable the pull up or pull down resistors for the GPIO port (PUPDR)
        // Since there are 2 configuration bits per bin, multiply the pin number
        // by 2 to get the actual bits we're supposed to set
        GPIOx->PUPDR |= (pupd << (2 * pin_no));
    }
}

//-----------------------------------------------------------------------------
//              Driver exposed APIs
//              
//-----------------------------------------------------------------------------

// Initializes the GPIO pin
//  GPIOx: GPIO port base address
//  gpio_pin_config: pointer to the pin config structure sent by the application
void hal_gpio_init(GPIO_TypeDef* GPIOx, gpio_pin_config_t* gpio_pin_config)
{
    if(GPIOx && gpio_pin_config)
    {
        // Configure the pin mode
        hal_gpio_configure_pin_mode(GPIOx, gpio_pin_config->pin, gpio_pin_config->mode);
        
        // Configure the pin speed
        hal_gpio_configure_pin_speed(GPIOx, gpio_pin_config->pin, gpio_pin_config->speed);
        
        // Configure the pin output type
        hal_gpio_configure_pin_output_type(GPIOx, gpio_pin_config->pin, gpio_pin_config->op_type);
        
        // Configure the pin pull up or pull down resistors
        hal_gpio_configure_pin_pupd(GPIOx, gpio_pin_config->pin, gpio_pin_config->pull);
    }
}

// Read a value from a given GPIO pin
//  GPIOx: GPIO port base address
//  pin_no: GPIO pin number
//  uint8_t: value read
uint8_t hal_gpio_read_from_pin(GPIO_TypeDef* GPIOx, uint16_t pin_no)
{
    uint8_t value = 0;
    
    // Read from the Input Data Register (IDR) then right-shift by the
    // value of pin_no to get the input status value of the pin to lsb
    if(GPIOx)
    {
        value = ((GPIOx->IDR >> pin_no) & 0x00000001);
    }
    
    return value;
}

// Write a value to a given GPIO pin
//  GPIOx: GPIO port base address
//  pin_no: GPIO pin number
//  value: value to be written
void hal_gpio_write_to_pin(GPIO_TypeDef* GPIOx, uint16_t pin_no, uint8_t value)
{
    if(GPIOx)
    {
        // Write to the pin while preserving existing values of other pins
        if(value)
        {
            GPIOx->ODR |= (1 << pin_no);
        }
        else
        {
            GPIOx->ODR &= ~(1 << pin_no);
        }
    }
}

// Set the alternate functionality for a given GPIO pin
//  GPIOx: GPIO port base address
//  pin_no: GPIO pin number
//  alt_func_value: altertate function value for which to configure the pin
void hal_gpio_set_alt_function(GPIO_TypeDef* GPIOx, uint16_t pin_no, uint16_t alt_func_value)
{
    if(GPIOx)
    {
        // AFR[0] is used for pins 0-7 while AFR[1] is used for pins 8-15
        if(pin_no <= 7)
        {
            // Each pin occupies 4 bits in the register so multiply by 4 and shift to set
            GPIOx->AFR[0] |= (alt_func_value << (pin_no * 4));
        }
        else
        {
            // Take the modulus of 8 from pin_no to convert pins 8-15 to 0-7 for ARF[1]
            GPIOx->AFR[1] |= (alt_func_value << ((pin_no % 8) * 4));
        }
    }
}

// Configure the interrupt for a given pin number
//  pin_no: GPIO pin number 
//  edge_sel: Triggering edge selection value of type "int_edge_sel_t" (rising edge, falling edge, both)
void hal_gpio_configure_interrupt(uint16_t pin_no, int_edge_sel_t edge_sel)
{
    if(edge_sel == INT_RISING_EDGE)
    {
        EXTI->RTSR |= 1 << pin_no;
    }
    else if(edge_sel == INT_FALLING_EDGE)
    {
        EXTI->FTSR |= 1 << pin_no;
    }
    else if(edge_sel == INT_RISING_FALLING_EDGE)
    {
        EXTI->RTSR |= 1 << pin_no;
        EXTI->FTSR |= 1 << pin_no;
    }
    else
    {
        // TODO
    }
}
    
// Enable the interrupt for a given pin number
//  pin_no: GPIO pin number 
//  irq_no: IRQ_NUMBER to be enabled in NVIC
void hal_gpio_enable_interrupt(uint16_t pin_no, IRQn_Type irq_no)
{
    // Enable the interrupt in the EXTI controller
    EXTI->IMR |= 1 << pin_no;
    
    // Enable the interrupt in the NVIC
    NVIC_EnableIRQ(irq_no);
}

// Clear the interrupt for a given pin number (else, the processor will keep being interrupted)
//  pin_no: GPIO pin number 
void hal_gpio_clear_interrupt(uint16_t pin_no)
{
    // If the interrupt bit is set, clear it so the processor stops getting interrupted
    if(EXTI->PR & (1 << pin_no))
    {
        EXTI->PR |= 1 << pin_no;
    }
        
}
