import os

from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine(os.getenv("DATABASE_URL"))
db = scoped_session(sessionmaker(bind=engine))

def main():
    flights = db.execute("SELECT * FROM REGISTRO").fetchall()
    for registro in REGISTRO:
        print(":id set to :estado at :time",
        {"id":registro.sensor_id,"estado":registro.estado,"tiempo":registro.tiempo})

if __name__ == "__main__":
    main()
