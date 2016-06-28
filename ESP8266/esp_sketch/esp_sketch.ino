// Dreist geklaut. Bei Ihm hier: https://github.com/ItKindaWorks/ESP8266
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Der ESP wird als Ausgabe entweder <DBG> und darauf folgende Debuggingmessages ausgeben 
// oder /topic/subtopic:message.
// Server
const char* mqtt_server;
// SSID
const char* ssid;
// WLAN passwd
const char* password;
// The Topics to subscribe to
const char* subTopics[255];
int topics = 0;
// von Beginn an ist der ESP erst einmal nicht konfiguriert.
boolean configured = false;
// Diese Funktion wird aufgerufen wenn etwas neues an ein abonniertes
// Thema gesendet wird.
void callback(char* topic, byte* payload, unsigned int length) {
	Serial.print(topic);
	Serial.print(":");
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
	// Konfiguration starten.
	configure();
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
		Serial.println("<DBG> WLAN connected");  
		Serial.print("<DBG> IP address: ");
		Serial.println(WiFi.localIP());
	}

	// Wenn wir verbunden sind dann soll es losgehen mit dem herstellen der
	// MQTT Verbindung
	if(WiFi.status() == WL_CONNECTED){
		Serial.println("<DBG> Attempting MQTT connection...");
		client.setServer(mqtt_server, 1883);
		//if connected, subscribe to the topic(s) we want to be notified about
		if (client.connect("esp8266")) {
			Serial.println("<DBG> MQTT Connected");
			for(int i = 0; i < topics; i++) {
				client.subscribe(subTopics[i]);
			}
		}
	}
}

void configure() {
	Serial.println("<DBG> need to be configured.");
        Serial.println("<DBG> Config me like this:<delim><mqtt_server><delim><ssid><delim><pw><delim><topic><delim><topic>");
	String config = "";
        while(configured == false) {
		while(Serial.available()) {
			char c = (char) Serial.read();
			if(c=='\r') {
				configured = true;
				String s_mqtt_server = "";
				String s_ssid = "";
				String s_pw = "";
				int delimcount = 1;
				int startOfTopics = 0;
				char delim = config.charAt(0);
				Serial.println("<DBG> System configured. Here is your config: ");
				for(int i = 1; i < config.length(); i++) {
					if(config.charAt(i) == delim) {
						delimcount++;
						if(delimcount == 4) {
							startOfTopics = i + 1;
						}
					} else {
						if(delimcount == 1) {
							s_mqtt_server.concat(config.charAt(i));
						}
						if(delimcount == 2) {
							s_ssid.concat(config.charAt(i));
						}
						if(delimcount == 3) {
							s_pw.concat(config.charAt(i));
						}
					}
				}
				topics = delimcount - 3;
				if(topics < 1) {
					Serial.println("<DBG> Sorry you subscribed to no topic.");
					abort();
				}
				String s_subTopics[topics];
				int topiccounter = 0;
				for(int i = startOfTopics; i < config.length(); i++) {
					if(config.charAt(i) == delim) {
						subTopics[topiccounter] = s_subTopics[topiccounter].c_str();
						topiccounter++;

					} else {
						s_subTopics[topiccounter].concat(config.charAt(i));
					}
				}
				subTopics[topics - 1] = s_subTopics[topics - 1].c_str();
				mqtt_server=s_mqtt_server.c_str();
				Serial.print("<DBG> MQTT-Server: ");
				Serial.println(mqtt_server);
				ssid=s_ssid.c_str();
				Serial.print("<DBG> SSID: ");
				Serial.println(ssid);
				Serial.print("<DBG> pw: ");
				password=s_pw.c_str();
				Serial.println(s_pw);
				Serial.print("<DBG> You subscribed to ");
				Serial.print(topics);
				Serial.println(" topics.");
				for(int i = 0; i < topics; i++) {
					Serial.print("<DBG> ");
					Serial.println(subTopics[i]);
				}
				
				// WLAN anschalten und verbinden
				WiFi.begin(ssid, password);
				delay(2000);
				reconnect();
			} else {
				config.concat(c);
			}
		}
        }
}
