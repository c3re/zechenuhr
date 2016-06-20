// Dreist geklaut. Bei Ihm hier: https://github.com/ItKindaWorks/ESP8266
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
// MQTT Server
#define MQTT_SERVER "192.168.2.29"
// WLAN-SSID in die sich der ESP verbinden soll.
const char* ssid = "essid";
// Passwort zum anmelden
const char* password = "pw";
// Topic das mitgelesen und ausgegeben werden soll.
char* subTopic = "/temp/drinnen";

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
PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);

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
		Serial.print("Connecting to ");
		Serial.println(ssid);
		// Das verbinden geschieht durch simples warten
		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			Serial.print(".");
		}
		
		// Sobald wir verbunden sind soll das ausgegeben werden
		Serial.println("");
		Serial.println("WiFi connected");  
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}

	// Wenn wir verbunden sind dann soll es losgehen mit dem herstellen der
	// MQTT Verbindung
	if(WiFi.status() == WL_CONNECTED){
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
		}
	}
}
