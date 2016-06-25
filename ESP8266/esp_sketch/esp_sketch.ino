// Dreist geklaut. Bei Ihm hier: https://github.com/ItKindaWorks/ESP8266
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
<<<<<<< HEAD

// Der ESP wird als Ausgabe entweder <DBG> und darauf folgende Debuggingmessages ausgeben oder /topic/subtopic:message.
char* mqtt_server = "192.168.1.116";
char* ssid = "Filebitch";
char* password = "ChaosComputerClub";
char* subTopic = "/test";
=======
// MQTT Server
#define MQTT_SERVER "192.168.2.29"
// WLAN-SSID in die sich der ESP verbinden soll.
const char* ssid = "essid";
// Passwort zum anmelden
const char* password = "pw";
// Topic das mitgelesen und ausgegeben werden soll.
char* subTopic = "/temp/drinnen";
>>>>>>> 874b18717c29289e0a66944b6d8bc6d099d5a9cd

// Diese Funktion wird aufgerufen wenn etwas neues an ein abonniertes
// Thema gesendet wird.
void callback(char* topic, byte* payload, unsigned int length) {
	// Einmal die ganze Payload durcharbeiten...
	for(int i = 0; i < length; i++) {
		// und Seriell ausgeben
		Serial.print((char)payload[i]);
	}
	// Am ende Newline.
	Serial.println("");
}

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, callback, wifiClient);

void setup() {
	// Output auf 115200 baud
	Serial.begin(115200);
	delay(100);
	// WLAN anschalten und verbinden
	WiFi.begin(ssid, password);
	// Die Funktion reconnect schaltet nacheinander WLAN an 
	// und verbindet dann den MQTT-Client
	reconnect();
	// ein bisschen warten
	delay(2000);
}

void loop() {
	// wenn WLAN und MQTT nicht verbunden dann neue connecten
	if (!client.connected() && WiFi.status() == 3) {
		reconnect();
	}
	// Hier kommt man erst hin wenn die Verbindung steht und wenn man hier ankommt
	// dann einfach nur die Verbindung halten.
	client.loop();
	// Dieses warten braucht der ESP anscheinend fuer die WLAN-Verwaltung.
	delay(10); 
}



void reconnect() {
	// Wenn nicht verbunden dann wieder verbinden
	if(WiFi.status() != WL_CONNECTED){
		Serial.print("<DBG> Connecting to ");
		Serial.println(ssid);
		// Das verbinden geschieht durch simples warten
		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
		}
<<<<<<< HEAD

		//print out some more debug once connected
		Serial.println("<DBG> WiFi connected");  
		Serial.print("<DBG> IP address: ");
=======
		
		// Sobald wir verbunden sind soll das ausgegeben werden
		Serial.println("");
		Serial.println("WiFi connected");  
		Serial.println("IP address: ");
>>>>>>> 874b18717c29289e0a66944b6d8bc6d099d5a9cd
		Serial.println(WiFi.localIP());
	}

	// Wenn wir verbunden sind dann soll es losgehen mit dem herstellen der
	// MQTT Verbindung
	if(WiFi.status() == WL_CONNECTED){
<<<<<<< HEAD
		Serial.println("<DBG> Attempting MQTT connection...");
		//if connected, subscribe to the topic(s) we want to be notified about
		if (client.connect("esp8266")) {
			Serial.println("<DBG> MQTT Connected");
			client.subscribe(subTopic);
=======
		// Solange nicht verbunden, weiterprobieren
		while (!client.connected()) {
			Serial.print("Attempting MQTT connection...");

			String clientName = "esp8266";
			// Wenn wir verbunden sind letztenendes noch unser Thema
			// abonnieren.
			if (client.connect((char*) clientName.c_str())) {
				Serial.println("\tMQTT Connected");
				client.subscribe(subTopic);
			}
>>>>>>> 874b18717c29289e0a66944b6d8bc6d099d5a9cd
		}
	}
}
