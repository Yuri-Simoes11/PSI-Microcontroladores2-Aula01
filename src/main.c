#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

// Configurações da pilha (stack) e prioridade das Threads
#define STACKSIZE 1024
#define PRIORITY 7

// Define os LEDs via Devicetree
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

// Especificação e verificação do LED0
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

// Especificação e verificação do LED1
#if DT_NODE_HAS_STATUS(LED1_NODE, okay)
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
#else
#error "Unsupported board: led1 devicetree alias is not defined"
#endif

// THREAD DO LED 0 (Pisca a cada 1000 ms)
void led0_thread_entry(void *p1, void *p2, void *p3)
{
    if (!gpio_is_ready_dt(&led0)) {
        printk("Error: LED0 device %s is not ready\n", led0.port->name);
        return;
    }

    if (gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE) < 0) {
        printk("Error: failed to configure LED0 pin\n");
        return;
    }

    printk("LED0 iniciado: alternando a cada 1000 ms\n");
    gpio_pin_set_dt(&led0, 0);
    while (1) {
        gpio_pin_set_dt(&led0, 1);
        k_msleep(10);
        gpio_pin_set_dt(&led0, 0);
        k_msleep(1000); // 1 segundo
    }
}

//THREAD DO LED 1 (Pisca a cada 500 ms)
void led1_thread_entry(void *p1, void *p2, void *p3)
{
    if (!gpio_is_ready_dt(&led1)) {
        printk("Error: LED1 device %s is not ready\n", led1.port->name);
        return;
    }

    if (gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE) < 0) {
        printk("Error: failed to configure LED1 pin\n");
        return;
    }

    printk("LED1 iniciado: alternando a cada 500 ms\n");
    gpio_pin_set_dt(&led1, 0);
    while (1) {
        gpio_pin_set_dt(&led1, 1);
        k_msleep(10);
        gpio_pin_set_dt(&led1, 0);
        k_msleep(350); 
    }
}

// Criação e inicialização das duas Threads independentes
K_THREAD_DEFINE(led0_thread_id, STACKSIZE, led0_thread_entry, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(led1_thread_id, STACKSIZE, led1_thread_entry, NULL, NULL, NULL, PRIORITY, 0, 0);

int main(void)
{
    printk("led0 e 1 piscadno\n");
    return 0;
}