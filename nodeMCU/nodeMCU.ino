/********************** Main Program Starts Here *****************************/
#include <ArduinoHttpClient.h>
#include <ESP8266WiFi.h>
#include "DHTesp.h"

const String ssid = "Pelota";
const String password = "Huawei01";

int sensores[3] = {D5, D6, D7};
int id[3] = {1, 2, 3};
bool estado[3];

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

//String serverAddress = "http://myhouse26.ddns.net";
//HttpClient client = HttpClient(wifi, serverAddress);

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

	client.get("/valoresArduino");
	String valores = client.responseBody();

	int i = valores.indexOf("#"); //0
	while (i != -1)
	{
		int j = valores.indexOf(":", i + 1); //13
		int k = valores.indexOf(";", i + 1); //20

		int ID = valores.substring(i + 1, j).toInt();
		String Estado = valores.substring(j + 1, k);

		for (int a = 0; a < 6; a++)
		{
			if (id[a] == ID)
			{
				if (Estado == "cerrado")
					estado[a] = true;
				else if (Estado == "abierto")
					estado[a] = false;
			}
		}
		i = valores.indexOf("#", i + 1); //-1
	}
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

			Serial.println(response);
		}
	}
}
