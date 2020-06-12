document.addEventListener('DOMContentLoaded', () => {
	setInterval (update, 3000), setInterval(alarma, 10000)
});

var armado = 0; 
var puerta_1, ventana_1, ventana_2, temperatura_1;

function update (){
	var reqURL = 'https://io.adafruit.com/api/v2/veintiseis/groups/default?x-aio-key=5515a3ad171c4034afe4ec86ed34999f'
	var req = new XMLHttpRequest();
	req.open('GET', reqURL, true);
	req.onreadystatechange = function () {
	if (req.readyState == 4)
		if  (req.status == 200) {
			obj = JSON.parse(req.responseText);
			obj.feeds.forEach((item) => {
				if (item.name == "command") {
					if ((item.last_value === "1") && (armado === 0)){
						connect.innerHTML = "Desbloquear Alarma <i class=\"fas fa-lock-open\"></i>";
						armado = 1;
					}
					if ((item.last_value === "0") && (armado === 1)){
						connect.innerHTML = "Bloquear Alarma <i class=\"fas fa-lock\"></i>";
						armado = 0;
					}
				}
				if (item.name == "sensor_1") {
					sensor_1.innerHTML = "Puerta Principal: " + item.last_value;
					puerta_1 = item.last_value;
				}
				if (item.name == "sensor_2") {
					sensor_2.innerHTML = "Ventana Sala: " + item.last_value;
					ventana_1 = item.last_value;
				}
				if (item.name == "sensor_3") {
					sensor_3.innerHTML = "Ventana Atras: " + item.last_value;
					ventana_2 = item.last_value;
				}
				if (item.name == "temperature") {
					sensor_4.innerHTML = "Temperatura de la Sala: " + item.last_value + "°/ " + humedad_sala;
				}
				if (item.name == "temperature_room") {
					sensor_5.innerHTML = "Temperatura del Cuarto: " + item.last_value + "°/ " + humedad_cuarto;
				}
				if (item.name == "humidity") {
					humedad_sala = item.last_value;
				}
				if (item.name == "humidity_room") {
					humedad_cuarto = item.last_value;
				}
			});
		}
		else {
	  	html.innerHTML = "<p>error "+req.status;
 		}
	};
	req.send(null);

}

function alarma () {
	if (armado && (puerta_1 == "abierto" || ventana_1 == "abierto" || ventana_2 == "abierto")) {
		alert("Zona abierta");
	}
}

function activar (){
	let xhr = new XMLHttpRequest();
	let url = "https://io.adafruit.com/api/v2/veintiseis/feeds/command/data.json?X-AIO-Key=5515a3ad171c4034afe4ec86ed34999f";
	
	xhr.open("POST", url, true);
	
	xhr.setRequestHeader("Content-Type", "application/json");

	xhr.onreadystatechange = function () { 
		if (xhr.readyState === 4 && xhr.status === 200) { 
			obj = JSON.parse(this.responseText);
			if (obj.feed_key == "command") {
				if ((obj.value === "1") && (armado === 0)){
					connect.innerHTML = "Desbloquear Alarma <i class=\"fas fa-lock-open\"></i>";
					armado = 1;
				}
				if ((obj.value === "0") && (armado === 1)){
					connect.innerHTML = "Bloquear Alarma <i class=\"fas fa-lock\"></i>";
					armado = 0;
				}
			}
		}
	}; 	
	var data = JSON.stringify({"value": ((armado+1)%2)})
	xhr.send(data);
}
