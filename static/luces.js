document.addEventListener('DOMContentLoaded', () => {
	setInterval (update, 3000)
});

var alarma;

function loadTable(data) {
	URL = '/tabla/' + data
	var xhttp = new XMLHttpRequest();
	xhttp.open("GET", URL, true);
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4)
			if (this.status == 200) {
				obj = JSON.parse(xhttp.responseText);
				var table = "<thead><tr>\
					<th>ID</th>\
					<th>Sensor</th>\
					<th>Estado</th>\
					<th>Hora</th>\
					</tr></thead><tbody>";
				obj.forEach((item) => {
					table = table + ("<tr>\
					<td>" + item.id + "</td>\
					<td>" + item.sensor + "</td>\
					<td>" + item.estado + "</td>\
					<td>" + item.tiempo + "</td>\
					</tr>");
				table = table + ("</tbody>")
				});
			document.getElementById("tabla").innerHTML = table;
			document.getElementById("html").innerHTML = "<button type=\"button\" onclick=\"hide_table() \">Esconder tabla</button>";
			}
			else
				tabla.innerHTML = "wtf :D"
			//$("#tabla").slideDown("slow");
	};
	xhttp.send(null);
}

function update (){
	var req = new XMLHttpRequest();
	req.open('GET', '/estadoLuces', true);
	req.onreadystatechange = function () {
		if (req.readyState == 4)
			if (req.status == 200) {
				obj = JSON.parse(req.responseText);
				obj.forEach((item) => {
					var id="l"+item.id.toString();
					document.getElementById(id).innerHTML = item.sensor + "<br>" + item.estado;
					if (item.estado == 'encendido')
						document.getElementById(id).className = "btn btn-success";
					else if (item.estado == 'apagado')
						document.getElementById(id).className = "btn btn-danger";
				});
			}
			else {
			html.innerHTML = "<p>error "+req.status + "</p>";
			}
	};
	req.send(null);
}

function hide_table () {
	document.getElementById("tabla").innerHTML = "";
}
