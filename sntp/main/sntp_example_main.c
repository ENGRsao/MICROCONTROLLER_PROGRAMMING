/* LwIP SNTP example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/apps/sntp.h"

#define EXAMPLE_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_WIFI_PASS CONFIG_WIFI_PASSWORD

static EventGroupHandle_t wifi_event_group;


static const char *TAG = "example";



char strftime_buf[64]; //buffer  to store time and date
time_t now; //variable to hold instance of time and date in epoch format
struct tm timeinfo; //time and date structure in human readable data

struct timeval timm; // struct to store epoch format for variable

void printCurrentTime(void *parameters)
{
	while(1)
	{
		  time(&now); // request for current time
		  localtime_r(&now, &timeinfo); //format into readable format
		  strftime(strftime_buf, sizeof(strftime_buf), "%d/%m/%Y %H:%M:%S", &timeinfo); //copy into the created buffer
		  ESP_LOGI(TAG, "The current date/time in Nigeria is: %s", strftime_buf);
		  vTaskDelay(5000 / portTICK_PERIOD_MS); // block the current running task for 5 seonds
	}
}

void app_main()
{

	timeinfo.tm_sec = 10; //set the seconds 0 - 59
	timeinfo.tm_min = 10; //set the minute 0 - 59
	timeinfo.tm_hour = 10; //set the hour 0 - 24
	timeinfo.tm_mday = 10; // set the day 0 - 31
	timeinfo.tm_mon  = 9; // set the month 0 - 11
	timeinfo.tm_year = 119; //set the year x - 1900
	timeinfo.tm_wday = 1; // day of the week 0 - 6

	timeinfo.tm_isdst = 0; //set the day time instance
	timm.tv_sec = mktime(&timeinfo); // make time to convert the human readable format to epoch format
	//timm.tv_sec = 1570477436; //can as well stored the epoch value directly in the struct
		timm.tv_usec = 0; //set epoch millisecond
		settimeofday(&timm, 0); // update the time of the day to the customize value
	time(&now);  //request current time and date
	localtime_r(&now, &timeinfo);  //format to the human readable format

    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo); // store in the required format

    xTaskCreate(printCurrentTime, "curent time Task", 4096, NULL,10, NULL); //create task to always print time at every instance of 5s

}

