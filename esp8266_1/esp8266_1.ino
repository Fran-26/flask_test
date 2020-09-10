/********************** Main Program Starts Here *****************************/

//22-08 Cambiado sensores 1=abierto 0=cerrado


#include <ArduinoHttpClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHTesp.h"

#define cantidadSensores 2
#define cantidadReles 2
#define sensor_T D2

const String ssid = "Pelota";
const String password = "Huawei01";

ESP8266WebServer server(80);

int sensores[cantidadSensores] = {D1, D2};
int id[cantidadSensores] = {3, 14};
bool estado[cantidadSensores];

int pulsadores[cantidadReles] = {D3, D4};
bool presionado[cantidadReles];
int reles[cantidadReles] = {D5, D6};
int id2[cantidadReles] = {11, 15};
bool estado2[cantidadReles];

DHTesp sensor_temperatura;
float temperatura = 0;
float humedad = 0;

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
	for (int i = 0; i<cantidadReles; i++)
	{
		if (server.argName(0) == String("estado" + String(i)))
		{
			estado2[i]= !estado2[i];
			digitalWrite(reles[i],estado2[i]);
			String url = "set/" + String(id2[i]);
			if (estado2[i])
				url += "/encendido";
			else
				url += "/apagado";

			client.get(url);
			client.stop();
		}
	}

	String html = "<html><head><title>Control De Salidas Programable</title>"
		"<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\">"
		"<style>\div {margin:auto; text-align:center; font-size:200%;}</style>"
		"</head><body><div>";
		
		html += "<br><h3>Estado de Salida 1:&nbsp;";
		if (digitalRead(reles[0]))
			html += "<form><button type=\"submit\" name=\"estado0\" class=\"btn btn-success\" >Encendido</button></h3></form>";
		else
			html += "<form><button type=\"submit\" name=\"estado0\" class=\"btn btn-dark\" > Apagado </button></h3></form>";

		html += "<br><h3>Estado de Salida 2:&nbsp;";
		if (digitalRead(reles[1]))
			html += "<form><button type=\"submit\" name=\"estado1\" class=\"btn btn-success\" >Encendido</button></h3></form>";
		else
			html += "<form><button type=\"submit\" name=\"estado1\" class=\"btn btn-dark\" > Apagado </button></h3></form>";

		html += "</div></body></html>";
		server.send(200, "text/html", html);
}

void setup()
{
	// set button pin as an input
	for (int i = 0; i < cantidadSensores; i++)
	{
		pinMode (sensores[i], INPUT_PULLUP);
	}
	for (int i = 0; i < cantidadReles; i++)
	{
		pinMode (pulsadores[i], INPUT_PULLUP);
		pinMode (reles[i], OUTPUT);
		//digitalWrite(reles[i], HIGH);
	}
	sensor_temperatura.setup(D0, DHTesp::DHT11);

	//Serial.begin(115200);
	WiFi.begin(ssid, password);
	while (WiFi.status() !=WL_CONNECTED) {
		delay(500);
	}
	//Serial.print("\n Conectado a: ");
	//Serial.println(WiFi.localIP());

	client.get("/valoresArduino");
	String valores;
	if (client.responseStatusCode() == 200)
		valores = client.responseBody();

	int i = valores.indexOf("#");
	while (i != -1)
	{
		int j = valores.indexOf(":", i + 1);
		int k = valores.indexOf(";", i + 1);

		int ID = valores.substring(i + 1, j).toInt();
		String Estado = valores.substring(j + 1, k);

		for (int a = 0; a < cantidadSensores; a++)
		{
			if (id[a] == ID)
			{
				if (Estado == "cerrado")
					estado[a] = false;
				else if (Estado == "abierto")
					estado[a] = true;
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
	String valores;
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval)
	{
		previousMillis = currentMillis;

		client.get("/estadoLucesArduino");
		if (client.responseStatusCode() == 200)
			valores = client.responseBody();

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
						estado2[a] = false;
					else if (Estado == "encendido")
						estado2[a] = true;
					digitalWrite (reles[a], estado2[a]);
				}
			}
			i = valores.indexOf("#", i + 1);
		}
		
		float t = sensor_temperatura.getTemperature();
		float h = sensor_temperatura.getHumidity();
		if (t > 0 && h > 0)
		{
			if (t > temperatura + .5 || t < temperatura - .5) 
			{
				temperatura = t;
				client.get("/set/4/" + String(temperatura));
				client.stop();
			}
			if (h > humedad + 5 || h < humedad - 5)
			{
				humedad = h;
				client.get("/set/5/" + String(humedad));
				client.stop();
			}
		}
	}

	for (int i = 0; i < cantidadSensores; i++)
	{
		if (digitalRead(sensores[i]) != estado[i])
		{
			estado[i]= !estado[i];

			String url = "set/" + String(id[i]);
			if (estado[i])
				url += "/abierto";
			else
				url += "/cerrado";

			client.get(url);
			client.stop();
		}
	}
	for (int i = 0; i < cantidadReles; i++)
	{
		if(!digitalRead(pulsadores[i]) && !presionado[i])
		{
			presionado[i]=true;
			estado2[i]= !estado2[i];
			digitalWrite(reles[i],estado2[i]);
			String url = "set/" + String(id2[i]);
			if (estado2[i])
				url += "/encendido";
			else
				url += "/apagado";

			client.get(url);
			client.stop();
		}
		if(digitalRead(pulsadores[i]) && presionado[i])
			presionado[i]=false;
	}
}
