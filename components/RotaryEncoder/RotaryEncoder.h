#ifndef RotaryEncoder_H
#define RotaryEncoder_H

#define ESP_INTR_FLAG_DEFAULT 0

typedef void (* FUNC_POINTER)(int, int, int);

typedef enum {
	EVENT_ChangePinAB,
	EVENT_ChangePinSW,
	EVENT_Inquiry,
} EVENT_ID_t;

typedef struct {
	EVENT_ID_t event;
	int16_t gpio_valA;
	int16_t gpio_valB;
	int16_t gpio_valSW;
	int16_t interruptAB;
	int16_t interruptSW;
	int16_t state;
	int16_t count;
} PARAMETER_t;

void initRotaryEncoder(int GpioPinA, int GpioPinB, int GpioPinSW, FUNC_POINTER p);
int readRotaryEncoder(int * count, int * sw, int * interrupt);

#endif
