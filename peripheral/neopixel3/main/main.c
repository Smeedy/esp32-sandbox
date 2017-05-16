#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/heap_regions.h"
#include "freertos/event_groups.h"

#include <esp_system.h>
#include <nvs_flash.h>
#include <esp_err.h>         // ESP32 ESP-IDF
#include <esp_log.h>         // ESP32 ESP-IDF
#include "esp_heap_alloc_caps.h"


#include "sdkconfig.h"

#include "tasks/wifi_sta_task.h"
#include "tasks/pixel_task.h"

#include "config.h"

#define TAG "[MAIN]"

int app_main() {
	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_LOGI(TAG,"free DRAM %u IRAM %u",esp_get_free_heap_size(),xPortGetFreeHeapSizeTagged(MALLOC_CAP_32BIT));

	app_event_group = xEventGroupCreate();

	xTaskCreate(pixel_task, "pixel_task", 12288, NULL, 5, NULL);
	xTaskCreate(wifi_sta_task, "wifi_sta_task", 12288, NULL, 5, NULL);

	return 0;
}

