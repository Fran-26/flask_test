import os

from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine(os.getenv("DATABASE_URL"))
db = scoped_session(sessionmaker(bind=engine))

def main():
    flights = db.execute("SELECT * FROM REGISTRO").fetchall()
    for registro in REGISTRO:
        print(f"{registro.sensor_id} set to {registro.estado} at {registro.tiempo}")

if __name__ == "__main__":
    main()
