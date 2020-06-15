import os

from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine("postgresql://pi:0000@localhost:5432/home")
db = scoped_session(sessionmaker(bind=engine))

def main():
    REGISTRO = db.execute("SELECT * FROM REGISTRO").fetchall()
    for registro in REGISTRO:
        #print(":id set to :estado at :time",
        #{"id":registro.sensor_id,"estado":registro.estado,"tiempo":registro.tiempo})
        print(f"{registro.sensor_id} set to {registro.estado} at {registro.tiempo}")

if __name__ == "__main__":
    main()
