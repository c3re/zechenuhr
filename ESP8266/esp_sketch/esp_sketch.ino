// Dreist geklaut. Bei Ihm hier: https://github.com/ItKindaWorks/ESP8266
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Der ESP wird als Ausgabe entweder <DBG> und darauf folgende Debuggingmessages ausgeben oder /topic/subtopic:message.
char* mqtt_server = "192.168.1.116";
char* ssid = "Filebitch";
char* password = "ChaosComputerClub";
char* subTopic = "/test";

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
		
		//print out some more debug once connected
		Serial.println("<DBG> WiFi connected");  
		Serial.print("<DBG> IP address: ");
		Serial.println(WiFi.localIP());
	}

	// Wenn wir verbunden sind dann soll es losgehen mit dem herstellen der
	// MQTT Verbindung
	if(WiFi.status() == WL_CONNECTED){
		Serial.println("<DBG> Attempting MQTT connection...");
		//if connected, subscribe to the topic(s) we want to be notified about
		if (client.connect("esp8266")) {
			Serial.println("<DBG> MQTT Connected");
			client.subscribe(subTopic);
		}
	}
}
