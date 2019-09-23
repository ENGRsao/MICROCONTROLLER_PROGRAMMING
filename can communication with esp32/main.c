/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/can.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <stdint.h>

#include "esp_log.h"
#include "mqtt_client.h"

esp_mqtt_client_handle_t client;

/* The examples use WiFi configuration that you can set via 'make menuconfig'.

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define CONFIG_ESP_WIFI_SSID "Photizzo_Office"
#define CONFIG_ESP_WIFI_PASSWORD "1r0bun007&008"
#define LED_PIN 27
#define BUTTON_PIN 34
#define BIT_0	( 1 << 0 )
#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY  3
#define CAN_TX 5
#define CAN_RX 35
#define CAN_MESSAGE_ID 0xA4
#define SLAVE_ID 0x125

typedef enum{
	TX_SEND_MESSAGE,
	TX_PING,
}tx_task_action;

typedef enum{
	RX_SEND_MESSAGE,
	RX_PING,
}rx_task_action;

static QueueHandle_t tx_Queue;
static QueueHandle_t rx_Queue;
static SemaphoreHandle_t start_task_sem;

int level = 0;
bool mqtt_connection = false;
/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about one event 
 * - are we connected to the AP with an IP? */
const int WIFI_CONNECTED_BIT = BIT_0;

can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(CAN_TX, CAN_RX, CAN_MODE_NORMAL);
can_timing_config_t t_config = CAN_TIMING_CONFIG_250KBITS();
can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();

can_message_t control_Power = {.identifier = CAN_MESSAGE_ID, .data_length_code = 1, .flags = CAN_MSG_FLAG_NONE};

static const can_message_t ping_message = {.identifier = CAN_MESSAGE_ID, .data_length_code = 0,
                                           .flags = CAN_MSG_FLAG_NONE, .data = {0, 0 , 0 , 0 ,0 ,0 ,0 ,0}};
static const can_message_t test_message = { .identifier = CAN_MESSAGE_ID, .data_length_code = 8,
											.flags = CAN_MSG_FLAG_NONE, .data = {'H','E','L','L','O','W','O','R'}};

static const char *TAG = "wifi station";

static int s_retry_num = 0;

uint8_t powerCommand = 0x0B;


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "got ip:%s", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        {
            if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
                esp_wifi_connect();
                xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
                s_retry_num++;
                ESP_LOGI(TAG,"retry to connect to the AP");
            }
            ESP_LOGI(TAG,"connect to the AP fail\n");
            break;
        }
    default:
        break;
    }
    return ESP_OK;
}

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
            ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
            mqtt_connection = true;
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            mqtt_connection = false;
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            mqtt_connection = false;
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            mqtt_connection = false;
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            mqtt_connection = true;
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            if (event->data[0] == 'n')
            {
            	level = 1;
            	gpio_set_level(LED_PIN,level);
            }
            else if (event->data[0] == 'f')
            {
            	level = 0;
            	gpio_set_level(LED_PIN,level);
            }
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            mqtt_connection = false;
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

void wifi_init_sta()
{
    s_wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL) );

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    ESP_LOGI(TAG, "connect to ap SSID:%s password:%s",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
}

static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
            .uri = "mqtt://m23.cloudmqtt.com",
            .event_handle = mqtt_event_handler,
    		.port = 17700,
    		.username = "wwrivypi",
    		.password = "C6r-EMOfyxb4",

        // .user_context = (void *)your_context
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
}


static void blink_setup(void *parameters)
{
	gpio_pad_select_gpio(LED_PIN);
	gpio_pad_select_gpio(BUTTON_PIN);
	/* Set the GPIO as a push/pull output */
	gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_PIN,GPIO_PULLUP_ONLY);
	int msg_id1;
	tx_task_action action;
	for (;;)
	{
		if (gpio_get_level(BUTTON_PIN) == 0)
		{
			vTaskDelay(100/portTICK_PERIOD_MS);
			if (gpio_get_level(BUTTON_PIN) == 0)
			{
				level = !level;
				while(gpio_get_level(BUTTON_PIN) == 0);

				msg_id1 = esp_mqtt_client_publish(client, "/topic/qos1", "i am publishing", 0, 1, 0);
				ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id1);

				action = TX_SEND_MESSAGE;
				if (level)
				{
					powerCommand = 0x0B;
				}
				else
				{
					powerCommand = 0x0C;
				}
				xQueueSendToFront(tx_Queue, &action, portMAX_DELAY);
			}

		}
		gpio_set_level(LED_PIN,level);
	}
}

static void CAN_Receive(void *parameters)
{
	while (1) {
	rx_task_action action;
	xQueueReceive(rx_Queue,&action,portMAX_DELAY);
	ESP_LOGI(TAG, "RUNNING CAN RECEIVE TASK");
	if (action == RX_SEND_MESSAGE)
	{
		can_message_t can_msg;
		can_receive(&can_msg,portMAX_DELAY);
		//if (can_msg.identifier == SLAVE_ID)
		//{
		   ESP_LOGI(TAG,"RECEIVEING message to client");
		//}
	}
	else if (action == RX_PING)
	{
		ESP_LOGI(TAG,"RECEIVE PING data");
	}
	}
}

static void CAN_transmit(void *parameters)
{
	while (1) {
	tx_task_action action;
	xQueueReceive(tx_Queue,&action,portMAX_DELAY);
	ESP_LOGI(TAG, "RUNNING CAN TRANSMIT TASK");
	if (action == TX_SEND_MESSAGE)
	{
		ESP_LOGI(TAG,"transmitting message to client");
		control_Power.data[0] = powerCommand;
		can_transmit(&control_Power, portMAX_DELAY);
	}
	else if (action == TX_PING)
	{
		ESP_LOGI(TAG,"sending PING data");
		can_transmit(&ping_message, portMAX_DELAY);
	}
	}
}
void vPublishTask(void *parameters)
{
	int msg_id;
	if (mqtt_connection)
	{
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
       msg_id = esp_mqtt_client_publish(client, "/topic/espData", "PC DATA MESSAGE", 0, 1, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
	}

}

void app_main()
{
    //Initialize NVS
    rx_Queue = xQueueCreate(1, sizeof(rx_task_action));
    tx_Queue = xQueueCreate(1, sizeof(tx_task_action));
    start_task_sem = xSemaphoreCreateBinary();
    rx_task_action rx_action;
    tx_task_action tx_action;
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    //Install CAN driver
    if (can_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        printf("Driver installed\n");
    } else {
        printf("Failed to install driver\n");
        return;
    }

    //Start CAN driver
    if (can_start() == ESP_OK) {
        printf("Driver started\n");
    } else {
        printf("Failed to start driver\n");
        return;
    }


    rx_action = RX_SEND_MESSAGE;
    tx_action = TX_SEND_MESSAGE;
    //xTaskCreate(vPublishTask,"publishing",1096,NULL,5,NULL);
    wifi_init_sta();
    mqtt_app_start();
    xTaskCreate(blink_setup,"blinky function",4096,NULL,5,NULL);
    xTaskCreatePinnedToCore(CAN_Receive,"CAN receive", 4096, NULL,8, NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(CAN_transmit,"CAN transmit", 4096, NULL,9, NULL, tskNO_AFFINITY);
    xQueueSend(tx_Queue, &tx_action , portMAX_DELAY);
    xQueueSend(rx_Queue, &rx_action , portMAX_DELAY);
}
