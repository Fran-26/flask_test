from flask import Flask, render_template, jsonify

from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine("postgresql://pi:0000@localhost:5432/home")
db = scoped_session(sessionmaker(bind=engine))

tablaEstado = []
valores = db.execute("SELECT * FROM ESTADO ORDER BY ID ASC").fetchall()
for id, registro in enumerate(valores):
	tablaEstado.append({"id": registro.id, "sensor": registro.sensor, "estado": registro.estado, "tipo": registro.tipo})

app = Flask(__name__)

@app.route("/")
def index():
	name= "Hello, world2!"
	return render_template("index.html", name = name)

@app.route("/<string:name>")
def hello(name):
	return "Hello, %s !" % name

@app.route("/more")
def more():
	return render_template("more.html")

@app.route("/camaras")
def camaras():
	return render_template("camaras.html")

@app.route("/sensores")
def sensores():
	return render_template("sensores.html")

@app.route("/luces")
def luces():
	return render_template("luces.html")

@app.route("/valores")
def valores():
	dictionary=[]
	for fila in tablaEstado:
		if fila["tipo"] != "luz":
			dictionary.append({"sensor": fila["sensor"], "estado": fila["estado"], "id": fila["id"]})
	return jsonify(dictionary)

@app.route("/valoresArduino")
def valoresArduino():
	dictionary = ""
	for fila in tablaEstado:
		if fila["tipo"] == "magnetico" or registro["tipo"] == "pir":
			dictionary += "#{id}:{estado};".format(id = fila["id"], estado = fila["estado"])
	return (dictionary)

#TO DO:convertir a POST
@app.route("/set/<string:id>/<string:estado>")
def set(id, estado):
	query=db.execute("SELECT * FROM SENSORES WHERE ID= {}".format(id)).fetchone()

	if query is None:
		return "Error, no existe el sensor numero {}".format(id)

	#if id == 6:
		#TODO

	db.execute("INSERT INTO REGISTRO (ID, ESTADO) VALUES ({id}, '{estado}');".format(id=id, estado=estado))
	db.execute("UPDATE SENSORES SET ESTADO='{estado}' WHERE ID={id};".format(id=id, estado=estado))
	db.commit()

	for i in range(len(tablaEstado)):
		if tablaEstado[i]["id"] == int(id):
			tablaEstado[i]["estado"] = estado

	x="{id} guardado {estado}".format(id=id, estado=estado)
	return (x)

@app.route("/tabla/<string:id>")
def tabla(id):
	query = db.execute("SELECT * FROM SENSORES JOIN REGISTRO ON REGISTRO.ID=SENSORES.ID WHERE SENSORES.ID={id} ORDER BY TIEMPO DESC LIMIT 20".format(id=id)).fetchall()

	if query is None:
		return "Error, no existe el sensor numero {}".format(id)

	dictionary=[]
	for id, registro in enumerate(query):
		dictionary.append({"sensor": registro.sensor, "id": registro.a, "estado" : registro.estado, "tiempo" : registro.tiempo})
	return jsonify(dictionary)

@app.route("/estadoLuces")
def estadoLuces():
	dictionary=[]
	for fila in tablaEstado:
		if fila["tipo"] == "luz":
			dictionary.append({"sensor": fila["sensor"], "estado": fila["estado"], "id": fila["id"]})
	return jsonify(dictionary)

@app.route("/estadoLucesArduino")
def estadoLucesArduino():
	dictionary = ""
	for fila in tablaEstado:
		if fila["tipo"] == "luz":
			dictionary += "#{id}:{estado};".format(id = fila["id"], estado = fila["estado"])
	return (dictionary)
