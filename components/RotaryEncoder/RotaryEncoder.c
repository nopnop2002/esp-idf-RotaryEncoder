/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "RotaryEncoder.h"

static char *TAG = "ENCODER";

const int8_t ENCODER_TABLE[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
int StatePinSW = 0;
uint8_t State = 0;
long Count = 0;
long InterruptCountAB = 0;
long InterruptCountSW = 0;
int _GpioPinA;
int _GpioPinB;
int _GpioPinSW;
FUNC_POINTER _callback;

static QueueHandle_t xQueueGpio = NULL;

static void IRAM_ATTR ChangePinAB(void *arg)
{
	PARAMETER_t param;
	param.event = EVENT_ChangePinAB;
	param.gpio_valA = gpio_get_level(_GpioPinA);
	param.gpio_valB = gpio_get_level(_GpioPinB);
	State = (State<<1) + param.gpio_valA;
	State = (State<<1) + param.gpio_valB;
	State = State & 0b00001111;
	if (ENCODER_TABLE[State] != 0) {
		Count += ENCODER_TABLE[State];
		InterruptCountAB++;
		param.interruptAB = InterruptCountAB;
		param.state = State;
		param.count = Count;
		xQueueSendFromISR(xQueueGpio, &param, NULL);
	}
}

void ChangePinSW(){
	PARAMETER_t param;
	param.event = EVENT_ChangePinSW;
	InterruptCountSW++;
	param.gpio_valSW = !gpio_get_level(_GpioPinSW);
	param.interruptSW = InterruptCountSW;
	xQueueSendFromISR(xQueueGpio, &param, NULL);
}

//typedef void (* FUNC_POINTER)(int, int, int);
void func(int evt, int count, int sw, FUNC_POINTER p){
	p(evt, count, sw);
}

// Wait event using task
static void gpio_task(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");
	PARAMETER_t param;
	while (1) {
		if (xQueueReceive(xQueueGpio, &param, portMAX_DELAY)) {
			ESP_LOGD(pcTaskGetName(NULL), "ivent=%d A=%d B=%d SW=%d state=%d interruptAB=%d interruptSW=%d count=%d", 
				param.event, param.gpio_valA, param.gpio_valB, param.gpio_valSW, param.state, param.interruptAB, param.interruptSW, param.count);
			func(param.event, param.count, param.gpio_valSW, _callback);
		}
	}
}

// Initialize RotaryEncoder
void initRotaryEncoder(int GpioPinA, int GpioPinB, int GpioPinSW, FUNC_POINTER callback) {
	_GpioPinA = GpioPinA;
	_GpioPinB = GpioPinB;
	_GpioPinSW = GpioPinSW;

	//zero-initialize the config structure.
	gpio_config_t io_conf;

	//interrupt of GpioPinA
	io_conf.intr_type = GPIO_INTR_ANYEDGE;
	//io_conf.intr_type = GPIO_INTR_NEGEDGE; // falling edge
	//bit mask of the pins, use GpioPinA here
	io_conf.pin_bit_mask = 1ULL<<_GpioPinA;
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_up_en = 1;
	io_conf.pull_down_en = 0;
	gpio_config(&io_conf);
	ESP_LOGD(TAG, "sizeof(io_conf.pin_bit_mask)=%d", sizeof(io_conf.pin_bit_mask));

	//interrupt of GpioPinB
	io_conf.intr_type = GPIO_INTR_ANYEDGE;
	//io_conf.intr_type = GPIO_INTR_NEGEDGE; // falling edge
	//bit mask of the pins, use GpioPinB here
	io_conf.pin_bit_mask = 1ULL<<_GpioPinB;
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_up_en = 1;
	io_conf.pull_down_en = 0;
	gpio_config(&io_conf);

	//interrupt of GpioPinSW
	io_conf.intr_type = GPIO_INTR_ANYEDGE;
	//io_conf.intr_type = GPIO_INTR_NEGEDGE; // falling edge
	//bit mask of the pins, use GpioPinSW here
	io_conf.pin_bit_mask = 1ULL<<_GpioPinSW;
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_up_en = 1;
	io_conf.pull_down_en = 0;
	gpio_config(&io_conf);

	//create a queue to handle gpio event from isr
	xQueueGpio = xQueueCreate(10, sizeof(PARAMETER_t));
	configASSERT( xQueueGpio );

	//install gpio isr service
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	//hook isr handler for specific gpio pin
	gpio_isr_handler_add(_GpioPinA, ChangePinAB, (void*) _GpioPinA);
	gpio_isr_handler_add(_GpioPinB, ChangePinAB, (void*) _GpioPinB);
	gpio_isr_handler_add(_GpioPinSW, ChangePinSW, (void*) _GpioPinSW);

	// With callback
	if (callback != NULL) {
		_callback = callback;
		xTaskCreate(gpio_task, "GPIO", 1024*4, NULL, 2, NULL);
	}
}

// Wait event using function
int readRotaryEncoder(int * count, int * sw, int * interrupt) {
	PARAMETER_t param;
	int _interrupt = 0;
	xQueueReceive(xQueueGpio, &param, portMAX_DELAY);
	ESP_LOGD(__FUNCTION__, "ivent=%d A=%d B=%d SW=%d state=%d interruptAB=%d interruptSW=%d count=%d", 
	param.event, param.gpio_valA, param.gpio_valB, param.gpio_valSW, param.state, param.interruptAB, param.interruptSW, param.count);
	_interrupt++;

	while (xQueueReceive(xQueueGpio, &param, 0)) {
		ESP_LOGD(__FUNCTION__, "ivent=%d A=%d B=%d SW=%d state=%d interruptAB=%d interruptSW=%d count=%d", 
		param.event, param.gpio_valA, param.gpio_valB, param.gpio_valSW, param.state, param.interruptAB, param.interruptSW, param.count);
		_interrupt++;
	}
	*count = param.count;
	*sw = param.gpio_valSW;
	*interrupt =  _interrupt;
	return param.event;
}


