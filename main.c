/*
 * @author Daniel Muelle Ramos
 * @date 03/01/2022
*/
 
/* Include required libraries */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/kobject.h>
#include <linux/time.h>

MODULE_LICENSE("GPL");

/* Definition of constants */
static unsigned int gpioRed = 16;
static unsigned int gpioBlue = 20;

static unsigned int gpioButton1 = 26;
static unsigned int gpioButton2 = 19;
static unsigned int gpioButton3 = 13;
static unsigned int gpioButton4 = 21;

static bool redOn = 0;
static bool blueOn = 0;

static unsigned int buttonPressed1 = 0;
static unsigned int buttonPressed2 = 0;
static unsigned int buttonPressed3 = 0;
static unsigned int buttonPressed4 = 0;

static unsigned int irqNumber1;
static unsigned int irqNumber2;
static unsigned int irqNumber3;
static unsigned int irqNumber4;

static irq_handler_t ebbgpio_irq_handler1(unsigned int irq, void *dev_id, struct pt_regs *regs);
static irq_handler_t ebbgpio_irq_handler2(unsigned int irq, void *dev_id, struct pt_regs *regs);
static irq_handler_t ebbgpio_irq_handler3(unsigned int irq, void *dev_id, struct pt_regs *regs);
static irq_handler_t ebbgpio_irq_handler4(unsigned int irq, void *dev_id, struct pt_regs *regs);


static int __init ebbgpio_init(void) {
    int result = 0;

    /* Print message on initialize to the kernel */
    printk(KERN_INFO "LEDs: Initializing LEDs main program LKM\n");

    /* Check if LED 1 has been properly assigned */
    if(!gpio_is_valid(gpioRed)) {
        printk(KERN_INFO "LEDs: Invalid gpioRed\n");
        return -ENODEV;
    }

    /* Check if LED 2 has been properly assigned */
    if(!gpio_is_valid(gpioBlue)) {
        printk(KERN_INFO "LEDs: Invalid gpioBlue\n");
        return -ENODEV;
    }

    /* Initialize as OFF all LEDs on start */
    redOn = false;
    gpio_request(gpioRed, "sysfs");
    gpio_direction_output(gpioRed, redOn);
    gpio_export(gpioRed, false);

    blueOn = false;
    gpio_request(gpioBlue, "sysfs");
    gpio_direction_output(gpioBlue, blueOn);
    gpio_export(gpioBlue, false);

    /* Initialize and declare buttons as IO pins */
    gpio_request(gpioButton1, "sysfs");
    gpio_direction_input(gpioButton1);
    gpio_set_debounce(gpioButton1, 200);
    gpio_export(gpioButton1, false);

    gpio_request(gpioButton2, "sysfs");
    gpio_direction_input(gpioButton2);
    gpio_set_debounce(gpioButton2, 200);
    gpio_export(gpioButton2, false);

    gpio_request(gpioButton3, "sysfs");
    gpio_direction_input(gpioButton3);
    gpio_set_debounce(gpioButton3, 200);
    gpio_export(gpioButton3, false);

    gpio_request(gpioButton4, "sysfs");
    gpio_direction_input(gpioButton4);
    gpio_set_debounce(gpioButton4, 200);
    gpio_export(gpioButton4, false);

    /* Mapping each gpio with its corresponding button */
    irqNumber1 = gpio_to_irq(gpioButton1);
    irqNumber2 = gpio_to_irq(gpioButton2);
    irqNumber3 = gpio_to_irq(gpioButton3);
    irqNumber4 = gpio_to_irq(gpioButton4);

    /* Linking each irq with its handler */
    result = request_irq(irqNumber1, (irq_handler_t) ebbgpio_irq_handler1, IRQF_TRIGGER_RISING, "ebb_gpio_handler1", NULL);
    result = request_irq(irqNumber2, (irq_handler_t) ebbgpio_irq_handler2, IRQF_TRIGGER_RISING, "ebb_gpio_handler2", NULL);
    result = request_irq(irqNumber3, (irq_handler_t) ebbgpio_irq_handler3, IRQF_TRIGGER_RISING, "ebb_gpio_handler3", NULL);
    result = request_irq(irqNumber4, (irq_handler_t) ebbgpio_irq_handler4, IRQF_TRIGGER_RISING, "ebb_gpio_handler4", NULL);


    return result;
}

static void __exit ebbgpio_exit(void) {
    /* Show messages informing of the buttons' status */
    printk(KERN_INFO "LEDs: Button 1 is currently: %d\n", gpio_get_value(gpioButton1));
    printk(KERN_INFO "LEDs: Button 2 is currently: %d\n", gpio_get_value(gpioButton2));
    printk(KERN_INFO "LEDs: Button 3 is currently: %d\n", gpio_get_value(gpioButton3));
    printk(KERN_INFO "LEDs: Button 4 is currently: %d\n", gpio_get_value(gpioButton4));

    /* Show messages informing of the number of times each button has been pressed */
    printk(KERN_INFO "LEDs: Button 1 was pressed %d times\n", buttonPressed1);
    printk(KERN_INFO "LEDs: Button 2 was pressed %d times\n", buttonPressed2);
    printk(KERN_INFO "LEDs: Button 3 was pressed %d times\n", buttonPressed3);
    printk(KERN_INFO "LEDs: Button 4 was pressed %d times\n", buttonPressed4);

    /* Unlinking pins and free up memory reserved for the irq */
    gpio_set_value(gpioRed, 0);
    gpio_set_value(gpioBlue, 0);
    gpio_unexport(gpioRed);
    gpio_unexport(gpioBlue);

    free_irq(irqNumber1, NULL);
    free_irq(irqNumber2, NULL);
    free_irq(irqNumber3, NULL);
    free_irq(irqNumber4, NULL);

    gpio_unexport(gpioButton1);
    gpio_unexport(gpioButton2);
    gpio_unexport(gpioButton3);
    gpio_unexport(gpioButton4);

    gpio_free(gpioRed);
    gpio_free(gpioBlue);

    gpio_free(gpioButton1);
    gpio_free(gpioButton2);
    gpio_free(gpioButton3);
    gpio_free(gpioButton4);

    /* Show message on exit */
    printk(KERN_INFO "LEDs: Goodbye form the LKM\n");
}

/* Handler for button 1 */
static irq_handler_t ebbgpio_irq_handler1(unsigned int irq, void *dev_id, struct pt_regs *regs) {
    /* Set LED as ON */
    blueOn = true;
    gpio_set_value(gpioBlue, blueOn);

    /* Increase counter of times button has been pressed */
    buttonPressed1++;

    /* Print message on press button */
    printk(KERN_INFO "LEDs: Button 1 has been pressed\n");

    return (irq_handler_t) IRQ_HANDLED;
}

/* Handler for button 2 */
static irq_handler_t ebbgpio_irq_handler2(unsigned int irq, void *dev_id, struct pt_regs *regs) {
    /* Set LED as OFF */
    blueOn = false;
    gpio_set_value(gpioBlue, blueOn);

    /* Increase counter of times button has been pressed */
    buttonPressed2++;

    /* Print message on press button */
    printk(KERN_INFO "LEDs: Button 2 has been pressed\n");

    return (irq_handler_t) IRQ_HANDLED;
}

/* Handler for button 3 */
static irq_handler_t ebbgpio_irq_handler3(unsigned int irq, void *dev_id, struct pt_regs *regs) {
    /* Set LED as ON */
    redOn = true;
    gpio_set_value(gpioRed, redOn);

    /* Increase counter of times button has been pressed */
    buttonPressed3++;

    /* Print message on press button */
    printk(KERN_INFO "LEDs: Button 3 has been pressed\n");

    return (irq_handler_t) IRQ_HANDLED;
}

/* Handler for button 4 */
static irq_handler_t ebbgpio_irq_handler4(unsigned int irq, void *dev_id, struct pt_regs *regs) {
    /* Set LED as OFF */
    redOn = false;
    gpio_set_value(gpioRed, redOn);

    /* Increase counter of times button has been pressed */
    buttonPressed4++;

    /* Print message on press button */
    printk(KERN_INFO "LEDs: Button 4 has been pressed\n");

    return (irq_handler_t) IRQ_HANDLED;
}

/* Set start and finish execution of the program */
module_init(ebbgpio_init);
module_exit(ebbgpio_exit);
