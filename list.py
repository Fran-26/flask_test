import os

from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine("postgresql://pi:0000@localhost:5432/home")
db = scoped_session(sessionmaker(bind=engine))

def main():
	REGISTRO = db.execute("SELECT * FROM ESTADO JOIN SENSORES ON SENSORES.ID=ESTADO.ID").fetchall()
	for registro in REGISTRO:
		print(registro.sensor, " set to ", registro.estado)
	id = "2"
	estado = "False"
	x="INSERT INTO REGISTRO (ID, ESTADO) VALUES ({id}, {estado});".format(id=id, estado=estado)
	print(x)
	db.execute(x)
	db.commit
	y="UPDATE ESTADO SET ESTADO={estado} WHERE ID={id};".format(id=id, estado=estado)
	print(y)
	db.execute(y)
	db.commit

if __name__ == "__main__":
	main()
