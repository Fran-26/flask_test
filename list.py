import os

from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine("postgresql://pi:0000@localhost:5432/home")
db = scoped_session(sessionmaker(bind=engine))

def main():
    REGISTRO = db.execute("SELECT * FROM REGISTRO JOIN SENSORES ON SENSORES.ID=REGISTRO.SENSOR_ID").fetchall()
    for registro in REGISTRO:
<<<<<<< HEAD
        print(registro.sensor_id, " set to ", registro.estado, " at ", registro.tiempo)
=======
        print(registro.sensor, " set to ", registro.estado, " at ", registro.tiempo)

>>>>>>> 666b41e8703e034c8df171e52edf96f12f523d85
if __name__ == "__main__":
    main()
