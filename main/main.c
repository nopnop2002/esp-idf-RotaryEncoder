/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "RotaryEncoder.h"

static char *TAG = "MAIN";


#if CONFIG_WITHOUT_CALLBACKE
void app_main()
{
	ESP_LOGI(TAG, "WITHOUT_CALLBACKE");
	initRotaryEncoder(CONFIG_GPIO_OUT_A, CONFIG_GPIO_OUT_B, CONFIG_GPIO_SWITCH, NULL);
	while (1) {
		int count; // Current count value
		int sw; // Current switch value
		int interrupt; // Interrupt occurrence count
		int event = readRotaryEncoder(&count, &sw, &interrupt);
		if (event == 0) ESP_LOGI(TAG, "cunt=%d interrupt=%d", count, interrupt);
		if (event == 1) ESP_LOGI(TAG, "sw=%d interrupt=%d", sw, interrupt);
	}
}
#endif


#if CONFIG_WITH_CALLBACKE
void callback1(int event, int count, int sw){
	if (event == 0) ESP_LOGI(TAG, "count=%d",count);
	if (event == 1) ESP_LOGI(TAG, "sw=%d",sw);
}

void app_main()
{
	ESP_LOGI(TAG, "WITH_CALLBACKE");
	initRotaryEncoder(CONFIG_GPIO_OUT_A, CONFIG_GPIO_OUT_B, CONFIG_GPIO_SWITCH, callback1);
	while (1) {
		vTaskDelay(10);
	}
}
#endif
