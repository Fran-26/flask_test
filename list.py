import os

from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine("postgresql://pi:0000@localhost:5432/home")
db = scoped_session(sessionmaker(bind=engine))

def main():
    REGISTRO = db.execute("SELECT * FROM REGISTRO JOIN SENSORES ON SENSORES.ID=REGISTRO.SENSOR_ID").fetchall()
    for registro in REGISTRO:
        print(registro.sensor_id, " set to ", registro.estado, " at ", registro.tiempo)

if __name__ == "__main__":
    main()
