/********************** Main Program Starts Here *****************************/
#include <ArduinoHttpClient.h>
#include <ESP8266WiFi.h>
#include "DHTesp.h"

const String ssid = "Pelota";
const String password = "Huawei01";

int sensores[3] = {D5, D6, D7};
int id[3] = {1, 2, 3};
boolean estado[3];

DHTesp sensor_temperatura;
int temperatura, humedad;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;       // will store last time LED was updated
const long interval = 10000;

String serverAddress = "192.168.100.14";
int port = 5000;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);

void setup()
{
	// set button pin as an input
	for (int i = 0; i < 3; i++)
	{
		pinMode (sensores[i], INPUT_PULLUP);
	}
	sensor_temperatura.setup(D3, DHTesp::DHT11);
	// start the serial connection
	Serial.begin(115200);
	WiFi.begin(ssid, password);
	while (WiFi.status() !=WL_CONNECTED) {
		delay(500);
	}
	Serial.print("\n Conectado a: ");
	Serial.println(WiFi.localIP());

	client.get("/valores");

	int statusCode = client.responseStatusCode();
	String response = client.responseBody();

	Serial.print("Status code: ");
	Serial.println(statusCode);
	Serial.print("Response: ");
	Serial.println(response);
	Serial.print("\n\n\n\n\n\n\n");

}

void loop()
{
	// check to see if it's time to blink the LED; that is, if the difference
	// between the current time and last time you blinked the LED is bigger than
	// the interval at which you want to blink the LED.
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval)
	{
		// save the last time you blinked the LED
		previousMillis = currentMillis;
	}

	for (int i = 0; i < 3; i++)
	{
		if (digitalRead(sensores[i]) != estado[i])
		{
			estado[i]= !estado[i];

			String url = "set/" + String(id[i]);
			if (estado[i])
				url += "/cerrado";
			else
				url += "/abierto";

			client.get(url);

			int statusCode = client.responseStatusCode();
			String response = client.responseBody();

			Serial.print("Status code: ");
			Serial.println(statusCode);
			Serial.print("Response: ");
			Serial.println(response);

			delay(1000);
		}
	}
}
