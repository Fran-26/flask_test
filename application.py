from flask import Flask, render_template

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