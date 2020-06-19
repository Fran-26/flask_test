document.addEventListener('DOMContentLoaded', () => {
	setInterval (update, 3000)
});
/*
function update (){
	var req = new XMLHttpRequest();
	req.open('GET', '/valores', true);
	req.onreadystatechange = function () {
	if (req.readyState == 4)
		if  (req.status == 200) {
			obj = JSON.parse(req.responseText);
			jsensor_1.innerHTML = "Puerta Principal: " + obj.puerta_1;
			jsensor_2.innerHTML = "Ventana Sala: " + obj.sala_1;
			jsensor_3.innerHTML = "Ventana Atras: " + obj.ventana_1;
			humedad_sala = "0";
			humedad_cuarto = "0";
			jsensor_4.innerHTML = "Temperatura de la Sala: " + "0" + "°/ " + humedad_sala;
			jsensor_5.innerHTML = "Temperatura del Cuarto: " + "0" + "°/ " + humedad_cuarto;
		}
		else {
	  	html.innerHTML = "<p>error "+req.status + "</p>";
 		}
	};
	req.send(null);

}
*/
function loadTable(data) {
	URL = '/tabla/' + data
	var xhttp = new XMLHttpRequest();
	xhttp.open("GET", URL, true);
	xhttp.onreadystatechange = function() {
	if (this.readyState == 4 && this.status == 200) {
		document.getElementById("html").innerHTML = this.responseText;
		}
	else
		html.innerHTML = "wtf :D"
	};
	xhttp.send(null);
}

function update (){
	var req = new XMLHttpRequest();
	req.open('GET', '/valores', true);
	req.onreadystatechange = function () {
		if (req.readyState == 4 && req.status == 200) {
			obj = JSON.parse(req.responseText);
			for (int i = 0; i < 5; i++) {
				document.getElementById("jsensor_"+i).innerHTML = obj.i.sensor + " " + obj.i.estado;
				if (obj.i.estado == 1){
					document.getElementById("jsensor_"+i).classList.add('btn btn-success');
					document.getElementById("jsensor_"+i).classList.toggle('btn btn-danger');
				}
				else{
					document.getElementById("jsensor_"+i).classList.add('btn btn-danger');
					document.getElementById("jsensor_"+i).classList.toggle('btn btn-success');
				}
			}
		}
		else {
		html.innerHTML = "<p>error "+req.status + "</p>";
		}
	};
	req.send(null);
}
