import pyodbc

driver = '{Microsoft Access Driver (*.mdb, *.accdb)}'
filepath = r'C:\Users\suhpa\Documents\milestone3.accdb'

myDataSources = pyodbc.dataSources()
access_driver = myDataSources['MS Access Database']

dbconn = pyodbc.connect(driver = access_driver, dbq=filepath, autocommit=True)
cursor = dbconn.cursor()


table_name = 'mqtt_server' # grab the table
query = "SELECT * FROM {}".format(table_name)

query_CreateTable = '''
    CREATE TABLE Server_STATS
    (   
        [DATETIME] datetime,
        [CLIENT_ID] varchar(50),
        [SEQ_NUM] integer,
        [TOTAL_CLIENTS] integer,
        [MSG_VALID] integer,
        [SUCESS_TIME] datetime,
        [FAILURE_TIME] datetime
    );
'''

cursor.execute(query_CreateTable)

query_CreateClientTable = '''
    CREATE TABLE Client_STATS
    (   
        [DATETIME] datetime,
        [CLIENT_ID] varchar(50),
        [SEQ_NUM] integer,
        [MSG_VALID] integer
    );
'''



cursor.execute(query_CreateClientTable)
dbconn.commit()