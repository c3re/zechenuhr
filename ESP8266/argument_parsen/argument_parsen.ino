// Teststring %192.168.2.29%mamulan%8ULJ9ntBLGt29rQtpP2TCdZf%/temp/drinnen%/temp/draussen%/time
boolean configured = false;
String config = "";
const char* mqtt_server;
const char* ssid;
const char* pw;
const char* subTopics[255];
int topics;
void setup() {
  // put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println("<DBG> need to be configured.");
	Serial.println("<DBG> Config me like this:<delim><mqtt_server><delim><ssid><delim><pw><delim><topic><delim><topic>");
}

void loop() {
	while(Serial.available() && configured == false) {
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
			String topic = "";
			int topiccounter = 0;
			for(int i = startOfTopics; i < config.length(); i++) {
				if(config.charAt(i) == delim) {
					Serial.println("Ich adde Topic: ");
					Serial.println(topic);
					subTopics[topiccounter] = topic.c_str();
					topic="";
					Serial.println(subTopics[topiccounter]);
					topiccounter++;
					Serial.println(topiccounter);
					
				} else {
					topic.concat(config.charAt(i));
				}
			}
			subTopics[topics - 1] = topic.c_str();
			mqtt_server=s_mqtt_server.c_str();
			Serial.print("<DBG> MQTT-Server: ");
			Serial.println(mqtt_server);
			ssid=s_ssid.c_str();
			Serial.print("<DBG> SSID: ");
			Serial.println(ssid);
			Serial.print("<DBG> pw: ");
			pw=s_pw.c_str();
			Serial.println(s_pw);
			Serial.print("<DBG> You subscribed to ");
			Serial.print(topics);
			Serial.println(" topics.");
			for(int i = 0; i < topics; i++) {
				Serial.print("<DBG>");
				Serial.println(subTopics[i]);
			}
		} else {
			config.concat(c);
		}
	}
}
