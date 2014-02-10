#define REFERENCE_VOLTAGE 3.3f
#define SENSOR_ID "sensorbot-a1"
#define SERVER_ADDRESS "ec2-50-18-226-240.us-west-1.compute.amazonaws.com"
#define TOPIC "office/sensors/a1/out"
#define PORT 1883

#define OUTPUT_PIN 13
#define TEMPERATURE_PIN A0
#define PHOTORESISTER_PIN A1

#include <Bridge.h>
#include <YunClient.h>
#include "PubSubClient.h"
#include "Thermosensor.h"
#include "Utils.h"

// Function prototypes.
void callback(char* topic, byte* payload, unsigned int length);

// Float conversion buffer.
char float_buffer[16];

// The encoded json string.
char json_buffer[] = "{\"f\":00000,\"l\":00000}";

// Lets the board bridge a connection to
// the internet.
YunClient yun;

// The thermosensor object that gets the temperature.
Thermosensor thermosensor(REFERENCE_VOLTAGE, TEMPERATURE_PIN);

// The MQTT class that lets the board publish
// to it's topic.
PubSubClient mqtt(SERVER_ADDRESS, PORT, callback, yun);

/*
 * The application start point that is called
 * when the board is powered or reset.
 */
void setup() {
	analogReference(EXTERNAL);

	// Initialize pin modes.
	pinMode(OUTPUT_PIN, OUTPUT);

	// Bridge to connect to the network.
	Bridge.begin();

	// Connect to the MQTT broker.
	mqtt.connect(SENSOR_ID);
}

/*
 * The application loop which is called
 * indefinatly for the rest of eternity or until
 * an idiot trips on the power cable.
 */
void loop() {
	// Get the temperature in farenheight and
	// store it as a string in the conversion buffer.
	dtostrf(thermosensor.farenheight(), 5, 1, float_buffer);

	// Insert the temperature into the json encoding.
	for (int i = 0; i < 5; i++) {
		json_buffer[i + 5] = float_buffer[i];
	}

	// Get the luminosity and
	// store it as a string in the conversion buffer.
	dtostrf(average_analog(100, PHOTORESISTER_PIN), 5, 1, float_buffer);

	// Insert the luminosity into the json encoding.
	for (int i = 0; i < 5; i++) {
		json_buffer[i + 15] = float_buffer[i];
	}

	// Publish the temperature, if this fails attempt to reconnect to the server.
	if (!mqtt.publish(TOPIC, json_buffer)) {
		bool is_disconnected = true;

		// Disconnect from the previous connection.
		mqtt.disconnect();

		// Attempt reconnects until a connection is made.
		while (is_disconnected) {
			delay(1000);

			if (mqtt.connect(SENSOR_ID)) {
				is_disconnected = false;
			} else {
				mqtt.disconnect();
			}
		}
	}
	
	// Blink the output light.
	digitalWrite(OUTPUT_PIN, HIGH);
	delay(10);
	digitalWrite(OUTPUT_PIN, LOW);

	// Pause for 5 seconds so the serial
	// console is not spammed.
	delay(4990);

	mqtt.loop();
}

/*
 * Fired by MQTT.
 */
void callback(char* topic, byte* payload, unsigned int length) {
	// MQTT callback.
}
