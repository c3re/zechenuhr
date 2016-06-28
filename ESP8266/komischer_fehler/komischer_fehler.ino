void setup() {
	Serial.begin(115200);
	const char* texte[10];
	String Text = "1";
	texte[0] = Text.c_str();
	Text = "2";
	texte[1] = Text.c_str();
	Serial.println(texte[0]);
	Serial.println(texte[1]);

}

void loop() {
  // put your main code here, to run repeatedly:

}
