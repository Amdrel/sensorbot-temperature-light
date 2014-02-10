#include <Arduino.h>
#include "Thermosensor.h"
#include "Utils.h"

Thermosensor::Thermosensor(float reference_voltage, const unsigned char pin) {
	Thermosensor::reference_voltage = reference_voltage;
	Thermosensor::analog_temperature_pin = pin;
}

int Thermosensor::raw() {
	return average_analog(100, Thermosensor::analog_temperature_pin);
}

float Thermosensor::voltage() {
	return (Thermosensor::raw() * reference_voltage) / 1024.0f;
}

float Thermosensor::celsius() {
	return (Thermosensor::voltage() - 0.5f) * 100.0f;
}

float Thermosensor::farenheight() {
	return (Thermosensor::celsius() * 9.0f / 5.0f) + 32.0f;
}
