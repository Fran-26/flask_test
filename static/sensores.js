document.addEventListener('DOMContentLoaded', () => {
	setInterval (update, 3000)
});

var armado = 0; 
var puerta_1, ventana_1, ventana_2, temperatura_1;

function update (){
	var reqURL = '{{ url_for('valores') }}'
	var req = new XMLHttpRequest();
	req.open('GET', reqURL, true);
	req.onreadystatechange = function () {
	if (req.readyState == 4)
		if  (req.status == 200) {
			obj = JSON.parse(req.responseText);
			sensor_1.innerHTML = "Puerta Principal: " + obj.puerta_1;
			sensor_2.innerHTML = "Ventana Sala: " + obj.sala_1;
			sensor_3.innerHTML = "Ventana Atras: " + obj.ventana_1;
			sensor_4.innerHTML = "Temperatura de la Sala: " + "0" + "°/ " + humedad_sala;
			sensor_5.innerHTML = "Temperatura del Cuarto: " + "0" + "°/ " + humedad_cuarto;
			humedad_sala = "0";
			humedad_cuarto = "0";
			);
		}
		else {
	  	html.innerHTML = "<p>error "+req.status;
 		}
	};
	req.send(null);

}