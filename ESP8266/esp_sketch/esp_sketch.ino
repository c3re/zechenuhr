#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Der ESP wird als Ausgabe entweder <DBG> und darauf folgende Debuggingmessages ausgeben oder /topic/subtopic:message.
char* mqtt_server = "192.168.1.116";
char* ssid = "Filebitch";
char* password = "ChaosComputerClub";
char* subTopic = "/test";


void callback(char* topic, byte* payload, unsigned int length);

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, callback, wifiClient);

void setup() {
	// Output auf 115200 baud
	Serial.begin(115200);
	delay(100);
	//start wifi subsystem
	WiFi.begin(ssid, password);
	//attempt to connect to the WIFI network and then connect to the MQTT server
	reconnect();
	//wait a bit before starting the main loop
	delay(2000);
}

void loop() {
	//reconnect if connection is lost
	if (!client.connected() && WiFi.status() == 3) {
		reconnect();
	}
	//maintain MQTT connection
	client.loop();
	//MUST delay to allow ESP8266 WIFI functions to run
	delay(10); 
}


void callback(char* topic, byte* payload, unsigned int length) {
	//convert topic to string to make it easier to work with String topicStr = topic;
	for(int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println("");
}

void reconnect() {
	//attempt to connect to the wifi if connection is lost
	if(WiFi.status() != WL_CONNECTED){
		Serial.print("<DBG> Connecting to ");
		Serial.println(ssid);
		//loop while we wait for connection
		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
		}

		//print out some more debug once connected
		Serial.println("<DBG> WiFi connected");  
		Serial.print("<DBG> IP address: ");
		Serial.println(WiFi.localIP());
	}

	//make sure we are connected to WIFI before attemping to reconnect to MQTT
	if(WiFi.status() == WL_CONNECTED){
		Serial.println("<DBG> Attempting MQTT connection...");
		//if connected, subscribe to the topic(s) we want to be notified about
		if (client.connect("esp8266")) {
			Serial.println("<DBG> MQTT Connected");
			client.subscribe(subTopic);
		}
	}
}
