#ifndef THERMOSENSOR_H
#define THERMOSENSOR_H

#include <Arduino.h>

class Thermosensor {
	public:
		/* Construct object with specified analog in. */
		Thermosensor(float reference_voltage, const unsigned char pin);

		/* Returns the raw analog output. */
		int raw();

		/* Returns the voltage of the raw value. */
		float voltage();

		/* Returns the temperature in celsius. */
		float celsius();

		/* Returns the temperature in farenheight. */
		float farenheight();
	private:
		/* The pin that the raw temperature data comes from. */
		unsigned char analog_temperature_pin;

		/* The reference voltage for the sensor. */
		float reference_voltage;
};

#endif
