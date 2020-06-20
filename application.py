from flask import Flask, render_template, jsonify

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
	dictionary=[]
	for id, registro in enumerate(valores):
		dictionary.append({"id": id, "sensor": registro.sensor, "estado" : registro.estado })
	return jsonify(dictionary)

#TO DO:convertir a POST
@app.route("/set/<string:id>/<string:estado>")
def set(id, estado):
	query=db.execute("SELECT * FROM SENSORES WHERE ID= {}".format(id)).fetchone()
	
	if query is None:
		return "Error, no existe el sensor numero {}".format(id)
		
	try:
		db.execute("INSERT INTO REGISTRO (ID, ESTADO) VALUES ({id}, '{estado}');".format(id=id, estado=estado))
		db.execute("UPDATE ESTADO SET ESTADO='{estado}' WHERE ID={id};".format(id=id, estado=estado))
		db.commit()
	except:
		return "Algo salio mal"
	x="{id} guardado {estado}".format(id=id, estado=estado)
	return (x)
	
@app.route("/tabla/<string:id>")
def tabla(id):
	#TODO
	return "hola :D"