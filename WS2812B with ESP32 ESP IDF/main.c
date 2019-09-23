

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/rmt.h"
#include <stdint.h>

#define BLINK_GPIO 27
#define BUTTON_PIN 34
#define INT_PIN 15
int blink_status = 0;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
	blink_status = !blink_status;
}


void hello_task(void *pvParameter)
{

    gpio_set_direction(INT_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(INT_PIN,GPIO_PULLUP_ONLY);
    gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    gpio_set_intr_type(INT_PIN,GPIO_INTR_POSEDGE);
    gpio_isr_handler_add(INT_PIN, gpio_isr_handler, (void*) INT_PIN);
	gpio_intr_enable(INT_PIN);
	while(1)
	{
		if (blink_status == 1)
		{
			printf("Hello world!\n");
	        vTaskDelay(100 / portTICK_RATE_MS);
		}
	}
}

void blinky(void *pvParameter)
{

    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_pad_select_gpio(BUTTON_PIN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN,GPIO_PULLUP_ONLY);
    int level = 0;
    while(1) {
        /* Blink off (output low) */
    	if (gpio_get_level(BUTTON_PIN) == 0){
    		level = !level;
    		while(gpio_get_level(BUTTON_PIN) == 0);
    	}
        gpio_set_level(BLINK_GPIO, level);
       // vTaskDelay(1000 / portTICK_RATE_MS);
        /* Blink on (output high) */
        //gpio_set_level(BLINK_GPIO, 1);
        //vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


// Configure these based on your project needs ********
#define LED_RMT_TX_CHANNEL RMT_CHANNEL_0
#define LED_RMT_TX_GPIO 18
// ****************************************************

#define BITS_PER_LED_CMD 24
#define LED_BUFFER_ITEMS ((NUM_LEDS * BITS_PER_LED_CMD))

// These values are determined by measuring pulse timing with logic analyzer and adjusting to match datasheet.
#define T0H 14  // 0 bit high time
#define T1H 36 //52  // 1 bit high time
#define T0L 36 //52  // low time for either bit
#define T1L 14
#define RED   0x000000
#define GREEN 0x0000FF
#define BLUE  0x000000

#ifndef NUM_LEDS
#define NUM_LEDS 3
#endif

// This structure is used for indicating what the colors of each LED should be set to.
// There is a 32bit value for each LED. Only the lower 3 bytes are used and they hold the
// Red (byte 2), Green (byte 1), and Blue (byte 0) values to be set.
struct led_state {
    uint32_t leds[NUM_LEDS];
};
// This is the buffer which the hw peripheral will access while pulsing the output pin
rmt_item32_t led_data_buffer[LED_BUFFER_ITEMS];

void setup_rmt_data_buffer(struct led_state new_state);

void ws2812_control_init(void)
{
  rmt_config_t config;
  config.rmt_mode = RMT_MODE_TX;
  config.channel = LED_RMT_TX_CHANNEL;
  config.gpio_num = LED_RMT_TX_GPIO;
  config.mem_block_num = 3;
  config.tx_config.loop_en = false;
  config.tx_config.carrier_en = false;
  config.tx_config.idle_output_en = true;
  config.tx_config.idle_level = 0;
  config.clk_div = 2;

  ESP_ERROR_CHECK(rmt_config(&config));
  ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));
}

void ws2812_write_leds(struct led_state new_state) {
  setup_rmt_data_buffer(new_state);
  ESP_ERROR_CHECK(rmt_write_items(LED_RMT_TX_CHANNEL, led_data_buffer, LED_BUFFER_ITEMS, false));
  ESP_ERROR_CHECK(rmt_wait_tx_done(LED_RMT_TX_CHANNEL, portMAX_DELAY));
}

void setup_rmt_data_buffer(struct led_state new_state)
{
  for (uint32_t led = 0; led < NUM_LEDS; led++) {
    uint32_t bits_to_send = new_state.leds[led];
    uint32_t mask = 1 << (BITS_PER_LED_CMD - 1);
    for (uint32_t bit = 0; bit < BITS_PER_LED_CMD; bit++) {
      uint32_t bit_is_set = bits_to_send & mask;
      led_data_buffer[led * BITS_PER_LED_CMD + bit] = bit_is_set ?
                                                      (rmt_item32_t){{{T1H, 1, T1L, 0}}} :
                                                      (rmt_item32_t){{{T0H, 1, T0L, 0}}};
      mask >>= 1;
    }
  }
}

void app_main()
{
	 ws2812_control_init();

	  struct led_state new_state;
	  new_state.leds[0] = RED;
	  new_state.leds[1] = GREEN;
	  new_state.leds[2] = BLUE;
	  ws2812_write_leds(new_state);
    //nvs_flash_init();

    //xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    //xTaskCreate(&blinky, "blinky", 512,NULL,5,NULL );
    //vTaskStartScheduler();
}
