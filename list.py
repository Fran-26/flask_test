import os

from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine(os.getenv('postgresql://pi:pi@localhost:5432/home'))
db = scoped_session(sessionmaker(bind=engine))

def main():
    flights = db.execute("SELECT * FROM REGISTRO").fetchall()
    for registro in REGISTRO:
        print(":id set to :estado at :time",
        {"id":registro.SENSOR_ID,"estado":registro.ESTADO,"tiempo":registro.TIEMPO})

if __name__ == "__main__":
    main()
