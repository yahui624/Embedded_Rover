import matplotlib.pyplot as plt 
import matplotlib.animation as animation 
from matplotlib import style 
import datetime
import pyodbc


# DB access set up
driver = '{Microsoft Access Driver (*.mdb, *.accdb)}'
filepath = r'C:\Users\Yahui\Documents\2020Spring\embedded\MS3_mqttServer\Milestone4\db_Rover_Server.accdb'
myDataSources = pyodbc.dataSources()
access_driver = myDataSources['MS Access Database']
dbconn = pyodbc.connect(driver=access_driver, dbq=filepath, autocommit=True)
cursor = dbconn.cursor()

time_label = []
m1_speed = []
m2_speed = []
m3_speed = []


style.use('fivethirtyeight')
fig, axs = plt.subplots(3, sharex=True)
fig.suptitle('PID Speed')

def plotting_PID(i, time_label, m1_speed, m2_speed, m3_speed): 
    cursor.execute('select * from PID_OUTPUT')
    print(cursor)
    values_set = cursor.fetchall()

    plt.cla()
    for row in values_set: 
        temp_time = datetime.datetime.strptime(row[0], ('%Y-%m-%d %H:%M:%S.%f')).strftime('%H:%M:%S.%f') # datetime.time 
        time_label.append(temp_time)
        m1_speed.append(row[1])
        m2_speed.append(row[2])
        m3_speed.append(row[3])

    time_label = time_label[-100:]
    m1_speed = m1_speed[-100:]
    m2_speed = m2_speed[-100:]
    m3_speed = m3_speed[-100:]

    axs[0].clear()
    axs[1].clear()
    axs[2].clear()
    axs[0].plot(time_label, m1_speed) # motor 1 
    axs[0].set(ylabel='motor 1')
    axs[1].plot(time_label, m2_speed) # motor 2
    axs[1].set(ylabel='motor 2')
    axs[2].plot(time_label, m3_speed) # motor 3
    axs[2].set(ylabel='motor 3')

    # plt.legend(loc='upper left')
    plt.gcf().autofmt_xdate()
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()

graph = animation.FuncAnimation(fig, plotting_PID, fargs=(time_label, m1_speed, m2_speed, m3_speed), interval=50) # run plotting every100 ms 
plt.show()