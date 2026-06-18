#ifndef GPIO_H
#define GPIO_H

void gpio_init_output(int pin);
void gpio_set(int pin);
void gpio_clear(int pin);

#endif /* GPIO_H */