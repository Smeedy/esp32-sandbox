#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <esp_err.h>         // ESP32 ESP-IDF
#include <esp_log.h>         // ESP32 ESP-IDF

#include <freertos/FreeRTOS.h>
#include "freertos/event_groups.h"

#include "sdkconfig.h"

#include "tasks/wifi_sta_task.h"
#include "tasks/pixel_task.h"

#include "config.h"


extern "C" int app_main(void) {
	ESP_ERROR_CHECK(nvs_flash_init());

	app_event_group = xEventGroupCreate();

	xTaskCreate(&pixel_task, "pixel_task", 2048, NULL, 5, NULL);
	xTaskCreate(&wifi_sta_task, "wifi_sta_task", 2048, NULL, 5, NULL);

	return 0;
}

