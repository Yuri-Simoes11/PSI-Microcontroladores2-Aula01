#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS 500

// Configurações da pilha (stack) e prioridade da Thread
#define STACKSIZE 1024
#define PRIORITY 7

// Define o LED usando Device Tree
#define LED0_NODE DT_ALIAS(led0)

// Verifica se o LED está definido no Device Tree
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

// Função de entrada da Thread (substitui o loop direto na main)
void led_thread_entry(void *p1, void *p2, void *p3)
{
    int ret;

    // Verifica se o device está pronto
    if (!gpio_is_ready_dt(&led)) {
        printk("Error: LED device %s is not ready\n", led.port->name);
        return;
    }

    // Configura o pino como saída
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }

    printk("LED blinking on %s pin %d (via Thread)\n", led.port->name, led.pin);

    while (1) {
        // Toggle do LED usando a APo Zeph
        gpio_pin_toggle_dt(&led);
        k_msleep(SLEEP_TIME_MS);
    }
}

// Cria e inicia a Thread automaticamente no boot do sistema
K_THREAD_DEFINE(led_thread_id, STACKSIZE, led_thread_entry, NULL, NULL, NULL, PRIORITY, 0, 0);

int main(void)
{
    printk("led piscadno\n");
    return 0;
}