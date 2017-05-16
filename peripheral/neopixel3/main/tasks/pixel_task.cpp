/*
 * pixel_task.cpp
 *
 *  Created on: 27 Apr 2017
 *      Author: smeedy
 */
#include "sdkconfig.h"

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <esp_err.h>         // ESP32 ESP-IDF
#include <esp_log.h>         // ESP32 ESP-IDF

#include "sdkconfig.h"
#include "WS2812.h"
#include "pixel_task.h"
#include "../config.h"

#define TAG "[PIXEL TASK]"

void pixel_task(void *data) {

	ESP_LOGI(TAG, "starting task");

	WS2812 ws2812 = WS2812(GPIO_NUM_19, 1, RMT_CHANNEL_0);

	pixel_t current, prev = {0,0,0};

	pixel_t red = {32, 0, 0 };
	pixel_t green = {0, 32, 0 };

	// init pixel
	ws2812.setPixel(0, red);
	ws2812.show();

	while (true) {

		int uxBits = xEventGroupWaitBits(
							app_event_group,	// The event group being tested.
							WIFI_DISCONNECTED_BIT | WIFI_CONNECTED_BIT,	// The bits within the event group to wait for.
							false,			// BIT_0 and BIT_4 should be cleared before returning.
							false,		// Don't wait for both bits, either bit will do.
							100 / portTICK_PERIOD_MS );	// Wait a maximum of 100ms for either bit to be set.

		ESP_LOGD(TAG, "uxBits: 0x%x", uxBits);

		if( ( uxBits & WIFI_CONNECTED_BIT ) != 0 ) {
			current = green;

		} else if ( ( uxBits & WIFI_DISCONNECTED_BIT ) != 0 ) {
			current = red;

		} else {
			// do nothing
		}

		if (current != prev) {
			ws2812.setPixel(0, current);
			ws2812.show();
		}



		vTaskDelay(10 / portTICK_PERIOD_MS);
	}


    vTaskDelete(NULL);
}


