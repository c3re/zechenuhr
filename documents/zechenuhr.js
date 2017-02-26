function setMinute(min) {
	var svg = window.document.getElementById("uhr");
	var svgDoc = svg.contentDocument;
	var mzeiger = svgDoc.getElementById("minutenzeiger");
	mzeiger.setAttributeNS(null, "transform", "rotate(" + min*6 + ",200,200)");
}

function setHour(h) {
	var svg = window.document.getElementById("uhr");
	var svgDoc = svg.contentDocument;
	var szeiger = svgDoc.getElementById("stundenzeiger");
	szeiger.setAttributeNS(null, "transform", "rotate(" + h*30 + ",200,200)");

}

function mqtt_led(ls) {
	var fields = ls.split(' ');
	var led = fields[0];
	var color = fields[1];
	if(led == 255) {
		for(i = 0; i < 60; i++) {
			set_color(i, color);
		}
	} else {
		set_color(led, color);
	}
}

function set_color(led, color) {
	var svg = window.document.getElementById("uhr");
	var svgDoc = svg.contentDocument;
	var myled = svgDoc.getElementById("led"+led);
	myled.setAttributeNS(null, "fill", color);
}

function set_time(ts) {
	var fields = ts.split(' ');
	var milliseconds = ((parseInt(fields[0]) + 3600 * parseInt(fields[1]) ) * 1000);
	var time = new Date();
	time.setTime(milliseconds);
	var minutes = time.getUTCMinutes();
	var hours = time.getUTCHours() + minutes / 60;
	setHour(hours);
	setMinute(minutes);

}

var client;

function start_mqtt() {
	client = new Paho.MQTT.Client("mqtt.lan", Number(8080), Math.random().toString(36).substring(7));
	client.onConnectionLost = onConnectionLost;
	client.onMessageArrived = onMessageArrived;
	client.connect({onSuccess:onConnect});
}

function onConnect() {
	console.log("onConnect");
	client.subscribe("huette/all/000/ccu/sensors/time");
	client.subscribe("waschkaue/eingangshalle/000/uhr/actuators/set_pixel");
}

function onConnectionLost(responseObject) {
	if (responseObject.errorCode !== 0) {
		console.log("onConnectionLost:"+responseObject.errorMessage);
	}
}

function onMessageArrived(message) {
	if(message.destinationName == "huette/all/000/ccu/sensors/time") {
		console.log("received Timestamp:" + message.payloadString);
		set_time(message.payloadString);

	} else {
		console.log("received LED command:" + message.payloadString);
		mqtt_led(message.payloadString);
	}
}

