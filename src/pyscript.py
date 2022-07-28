import serial
from datetime import datetime
serial_port="/dev/ttyACM0"
baud_rate=9600
path="results.txt"
ser=serial.Serial(serial_port,baud_rate)
with open(path,mode='a') as results:
    while True:
        line1 = ser.readline();
        line1 = line1.decode("utf-8") #ser.readline returns a binary, convert to string
        line2="    {}\n".format(datetime.now())
        line=line1[:-1]+line2
        print(line);
        results.write(line);