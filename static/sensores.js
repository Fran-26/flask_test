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
			obj = JSON.parse(req.responseText);
			var table = "<table><thead><tr>\
				<th>ID</th>\
				<th>Sensor</th>\
				<th>Estado</th>\
				<th>Hora</th>\
				</tr></thead><tbody>";
			obj.forEach((item) => {
				table.append("<tr>\
				<td>" + obj.id + "</td>\
				<td>" + obj.sensor + "</td>\
				<td>" + obj.estado + "</td>\
				<td>" + obj.tiempo + "</td>\
				</tr>");
			});
			table.append("</tbody>")
		document.getElementById("html").innerHTML = this.responseText;
		}
	else
		tabla.innerHTML = "wtf :D"
	};
	xhttp.send(null);
}

function update (){
	var req = new XMLHttpRequest();
	req.open('GET', '/valores', true);
	req.onreadystatechange = function () {
		if (req.readyState == 4 && req.status == 200) {
			obj = JSON.parse(req.responseText);
			obj.forEach((item) => {
				var id="s"+item.id.toString();
				document.getElementById(id).innerHTML = item.sensor + "<:br>" + item.estado;
				if (item.estado == 'cerrado')
					document.getElementById(id).className = "btn btn-success";
				else if (item.estado == 'abierto')
					document.getElementById(id).className = "btn btn-danger";
			});
		}
		else {
		html.innerHTML = "<p>error "+req.status + "</p>";
		}
	};
	req.send(null);
}
