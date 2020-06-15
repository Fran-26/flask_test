import os

from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine("postgresql://pi:0000@localhost:5432/home")
db = scoped_session(sessionmaker(bind=engine))

def main():
    REGISTRO = db.execute("SELECT * FROM REGISTRO").fetchall()
    for registro in REGISTRO:
        print("%s set to %s at %s",(registro.sensor_id, registro.estado, registro.tiempo))

if __name__ == "__main__":
    main()
