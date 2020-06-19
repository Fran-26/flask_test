from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine("postgresql://pi:0000@localhost:5432/home")
db = scoped_session(sessionmaker(bind=engine))

def main():
	valores = db.execute("SELECT * FROM ESTADO JOIN SENSORES ON SENSORES.ID=ESTADO.ID").fetchall()
	dictionary={}
	for id, registro in enumerate(valores):
		dictionary[id] = {"sensor": registro.sensor, "estado" : registro.estado }
		'''if registro.estado:
			dictionary [registro.sensor] = "cerrado"
		else:
			dictionary [registro.sensor] = "abierto" '''
		print(dictionary)

if __name__ == "__main__":
    main()
