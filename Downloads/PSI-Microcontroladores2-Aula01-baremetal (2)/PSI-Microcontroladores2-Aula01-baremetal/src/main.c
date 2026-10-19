#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

// Define o LED usando Device Tree
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

// Verifica se o LED está definido no Device Tree
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
static const struct gpio_dt_spec ledv = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif 

// Verifica se o LED está definido no Device Tree
#if DT_NODE_HAS_STATUS(LED1_NODE, okay)
static const struct gpio_dt_spec ledb = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
#else
#error "Unsupported board: led1 devicetree alias is not defined"
#endif

void main(void)
{
    int ret;

    // Verifica se o device está pronto
    if (!gpio_is_ready_dt(&ledv)) {
        printk("Error: LED device %s is not ready\n", ledv.port->name);
        return;
    }

    // Configura o pino como saída
    ret = gpio_pin_configure_dt(&ledv, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }

    // Verifica se o device está pronto
    if (!gpio_is_ready_dt(&ledb)) {
        printk("Error: LED device %s is not ready\n", ledv.port->name);
        return;
    }

    // Configura o pino como saída
    ret = gpio_pin_configure_dt(&ledb, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }

    int cont = 0; // Inicializa variavel para contagem
    // Condição: um led a cada 1s e outro a cada 350ms
    // o contador soma a cada 50ms. Logo, quando chega em algum múltiplo
    // de 7, ele altera um led, e quando chega no múltiplo de 20,
    // altera do outro led (350ms e 1000ms).

    while (1) {
        cont++;

        if(cont % 7 == 0){
            gpio_pin_set_dt(&ledv, 1);
        }

        if(cont % 20 == 0){
            gpio_pin_set_dt(&ledb, 1);
        }

        k_msleep(50);
        gpio_pin_set_dt(&ledv, 0);
        gpio_pin_set_dt(&ledb, 0);

    }
}