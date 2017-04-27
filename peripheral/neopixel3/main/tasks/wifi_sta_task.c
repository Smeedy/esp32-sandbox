/*
 * wifi_sta_task.c
 *
 *  Created on: 26 Apr 2017
 *      Author: smeedy
 */

#include <string.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/event_groups.h"
#include "esp_wifi.h"

#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"

#include "../config.h"


#define DEFAULT_SSID CONFIG_WIFI_SSID
#define DEFAULT_PWD CONFIG_WIFI_PASSWORD

#if CONFIG_POWER_SAVE_MODEM
#define DEFAULT_PS_MODE WIFI_PS_MODEM
#elif CONFIG_POWER_SAVE_NONE
#define DEFAULT_PS_MODE WIFI_PS_NONE
#else
#define DEFAULT_PS_MODE WIFI_PS_NONE
#endif /*CONFIG_POWER_SAVE_MODEM*/

const int CONNECTED_BIT = WIFI_CONNECTED_BIT;
const int DISCONNECTED_BIT = WIFI_DISCONNECTED_BIT;


#define TAG "[WIFI STA TASK]"


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
    	ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
    	ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
        xEventGroupSetBits(app_event_group, CONNECTED_BIT);
        ESP_LOGI(TAG, "got ip:%s\n",
        		ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
    	ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
        esp_wifi_connect();
        xEventGroupSetBits(app_event_group, DISCONNECTED_BIT);
        xEventGroupClearBits(app_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

static void init_wifi(void) {

    tcpip_adapter_init();

    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
		.sta = {
			.ssid = DEFAULT_SSID,
			.password = DEFAULT_PWD
		},
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "esp_wifi_set_ps().");
    esp_wifi_set_ps(DEFAULT_PS_MODE);
}

void wifi_sta_task(void* data) {

	ESP_LOGI(TAG, "starting task");

	init_wifi();

//	while(true){
//		ESP_LOGI(TAG, "...");
//		vTaskDelay(1000 / portTICK_PERIOD_MS);
//	}

    vTaskDelete(NULL);
}
