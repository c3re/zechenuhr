#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define MQTT_SERVER "192.168.2.29"
const char* ssid = "essid";
const char* password = "pw";
char* subTopic = "/temp/drinnen";


void callback(char* topic, byte* payload, unsigned int length);

WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);

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
		Serial.print("Connecting to ");
		Serial.println(ssid);
		//loop while we wait for connection
		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			Serial.print(".");
		}

		//print out some more debug once connected
		Serial.println("");
		Serial.println("WiFi connected");  
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}

	//make sure we are connected to WIFI before attemping to reconnect to MQTT
	if(WiFi.status() == WL_CONNECTED){
		// Loop until we're reconnected to the MQTT server
		while (!client.connected()) {
			Serial.print("Attempting MQTT connection...");

			// Generate client name based on MAC address and last 8 bits of microsecond counter
			String clientName = "esp8266";

			//if connected, subscribe to the topic(s) we want to be notified about
			if (client.connect((char*) clientName.c_str())) {
				Serial.println("\tMQTT Connected");
				client.subscribe(subTopic);
			}
			//otherwise print failed for debugging
			else{
				Serial.println("\tFailed."); 
				abort();
			}
		}
	}
}
