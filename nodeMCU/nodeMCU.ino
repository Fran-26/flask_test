/********************** Main Program Starts Here *****************************/
#include <ArduinoHttpClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHTesp.h"

#define cantidad 3
#define cantidad2 2

const String ssid = "Pelota";
const String password = "Huawei01";

ESP8266WebServer server(80);

int sensores[cantidad] = {D5, D6, D7};
int id[cantidad] = {1, 2, 3};
bool estado[cantidad];

int reles[cantidad2] = {D3, D4};
int id2[cantidad2] = {8, 9};
bool estado2[cantidad2];

DHTesp sensor_temperatura;
int temperatura, humedad;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;       // will store last time LED was updated
const long interval = 3000;

String serverAddress = "192.168.100.14";
int port = 5000;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);

//String serverAddress = "http://myhouse26.ddns.net";
//HttpClient client = HttpClient(wifi, serverAddress);

void handleRoot() {
	if (server.argName(0) == "estado")
	{
		estado2[0]= !estado2[0];
		digitalWrite(reles[0],estado2[0]);
		String url = "set/" + String(id2[0]);
		if (estado2[0])
			url += "/apagado";
		else
			url += "/encendido";

		client.get(url);

		int statusCode = client.responseStatusCode();
		String response = client.responseBody();
	}
	if (server.argName(0) == "estado2")
	{
		estado2[1]= !estado2[1];
		digitalWrite(reles[1],estado2[1]);
		String url = "set/" + String(id2[1]);
		if (estado2[1])
			url += "/apagado";
		else
			url += "/encendido";

		client.get(url);

		int statusCode = client.responseStatusCode();
		String response = client.responseBody();
	}
	
	String html = "<html><head><title>Control De Salidas Programable</title>"
		"<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\">"
		"<style>\div {margin:auto; text-align:center; font-size:200%;}</style>"
		"</head><body><div><h3>Estado de Salida 1:&nbsp;";
		if (!digitalRead(reles[0]))
			html += "<form><button type=\"submit\" name=\"estado\" class=\"btn btn-success\" >Encendido</button></h3></form>";
		else
			html += "<form><button type=\"submit\" name=\"estado\" class=\"btn btn-dark\" > Apagado </button></h3></form>";
		html += "<br><h3>Estado de Salida 2:&nbsp;";
		if (!digitalRead(reles[1]))
			html += "<form><button type=\"submit\" name=\"estado2\" class=\"btn btn-success\" >Encendido</button></h3></form>";
		else
			html += "<form><button type=\"submit\" name=\"estado2\" class=\"btn btn-dark\" > Apagado </button></h3></form>";
		html += "</div></body></html>";
		server.send(200, "text/html", html);
}

void setup()
{
	// set button pin as an input
	for (int i = 0; i < cantidad; i++)
	{
		pinMode (sensores[i], INPUT_PULLUP);
	}
	for (int i = 0; i < cantidad2; i++)
	{
		pinMode (reles[i], OUTPUT);
	}
	//sensor_temperatura.setup(D3, DHTesp::DHT11);

	Serial.begin(115200);
	WiFi.begin(ssid, password);
	while (WiFi.status() !=WL_CONNECTED) {
		delay(500);
	}
	Serial.print("\n Conectado a: ");
	Serial.println(WiFi.localIP());

	client.get("/valoresArduino");
	String valores = client.responseBody();

	int i = valores.indexOf("#");
	while (i != -1)
	{
		int j = valores.indexOf(":", i + 1);
		int k = valores.indexOf(";", i + 1);

		int ID = valores.substring(i + 1, j).toInt();
		String Estado = valores.substring(j + 1, k);

		for (int a = 0; a < cantidad; a++)
		{
			if (id[a] == ID)
			{
				if (Estado == "cerrado")
					estado[a] = true;
				else if (Estado == "abierto")
					estado[a] = false;
			}
		}
		i = valores.indexOf("#", i + 1);
	}
	
	server.on("/", handleRoot);
	server.begin();
}

void loop()
{
	server.handleClient();
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval)
	{
		previousMillis = currentMillis;

		client.get("/estadoLucesArduino");
		String valores = client.responseBody();

		int i = valores.indexOf("#");
		while (i != -1)
		{
			int j = valores.indexOf(":", i + 1);
			int k = valores.indexOf(";", i + 1);

			int ID = valores.substring(i + 1, j).toInt();
			String Estado = valores.substring(j + 1, k);

			for (int a = 0; a < 6; a++)
			{
				if (id2[a] == ID)
				{
					if (Estado == "apagado")
						estado2[a] = true;
					else if (Estado == "encendido")
						estado2[a] = false;
					digitalWrite (reles[a], estado2[a]);
					//Serial.println(String(id2[a]) + "-->" + String(sensores[a]) + ":" + String(estado2[a]));
				}
			}	
			i = valores.indexOf("#", i + 1);
		}
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

			//Serial.println(response);
		}
	}
}
