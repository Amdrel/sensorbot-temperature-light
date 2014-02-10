#include <Arduino.h>
#include "Utils.h"

float average_analog(int count, char pin) {
	float average = 0.0f;

	for (int i = 0; i < count; i++) {
		average += analogRead(pin);
	}

	return average / count;
}
