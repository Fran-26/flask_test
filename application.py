from flask import Flask, render_template
from flask import jsonify

from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine("postgresql://pi:0000@localhost:5432/home")
db = scoped_session(sessionmaker(bind=engine))

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
	valores = db.execute("SELECT * FROM ESTADO JOIN SENSORES ON SENSORES.ID=ESTADO.ID").fetchall()
	#valores2 = db.execute("select json_agg(t) from (SELECT * FROM ESTADO JOIN SENSORES ON SENSORES.ID=ESTADO.ID) t").fetchall()
	
	dictionary = {}
	for registro in valores:
		if registro.estado:
			dictionary [registro.sensor] = "cerrado"
		else:
			dictionary [registro.sensor] = "abierto"
		print(registro.sensor, " set to ", registro.estado)

	return jsonify(dictionary)
	
@app.route("/set/<int:id>/<string:estado1>")
def set(id, estado1):
	x="hola"
	return (x)
