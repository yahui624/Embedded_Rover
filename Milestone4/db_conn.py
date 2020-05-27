import pyodbc

driver = '{Microsoft Access Driver (*.mdb, *.accdb)}'
filepath = r'C:\Users\Yahui\Documents\2020Spring\embedded\MS3_mqttServer\Milestone4\db_Rover_Server.accdb'

myDataSources = pyodbc.dataSources()
access_driver = myDataSources['MS Access Database']

dbconn = pyodbc.connect(driver = access_driver, dbq=filepath, autocommit=True)
cursor = dbconn.cursor()


# table_name = 'mqtt_server' # grab the table
# query = "SELECT * FROM {}".format(table_name)

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

# cursor.execute(query_CreateTable)

query_CreateClientTable = '''
    CREATE TABLE Client_STATS
    (   
        [DATETIME] datetime,
        [CLIENT_ID] varchar(50),
        [SEQ_NUM] integer,
        [MSG_VALID] integer
    );
'''

# cursor.execute(query_CreateClientTable)

query_CreateSpeedTable = '''
    CREATE TABLE SPEED_CMD
    (   
        [STATUS] varchar(50), 
        [SEQ_NUM] integer, 
        [M1_SPEED] integer, 
        [M1_DIRECTION] varchar(50), 
        [M2_SPEED] integer, 
        [M2_DIRECTION] varchar(50), 
        [M3_SPEED] integer, 
        [M3_DIRECTION] varchar(50)
    );
'''

# cursor.execute(query_CreateSpeedTable)


query_CreatePIDTable = '''
    CREATE TABLE PID_OUTPUT
    (   
        [TIME] varchar(50), 
        [M1_SPEED] integer, 
        [M2_SPEED] integer, 
        [M3_SPEED] integer
    );
'''

cursor.execute(query_CreatePIDTable)
dbconn.commit()