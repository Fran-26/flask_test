from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker
import json

engine = create_engine("postgresql://pi:0000@localhost:5432/home")
db = scoped_session(sessionmaker(bind=engine))

def main():
	valores = db.execute("SELECT * FROM ESTADO JOIN SENSORES ON SENSORES.ID=ESTADO.ID").fetchall()
	dictionary={}
	for id, registro in enumerate(valores):
		dictionary[id] = {"sensor": registro.sensor, "estado" : registro.estado }
	#print(dictionary)

	dictionary2={}
	for id, registro in enumerate(valores):
		dictionary2.sensores["id"] = {"sensor": registro.sensor, "estado" : registro.estado }
	x=json.dumps(dictionary2)
	print(x)

if __name__ == "__main__":
	main()
