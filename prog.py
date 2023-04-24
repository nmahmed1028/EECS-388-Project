import serial
import csv

ser1 = serial.Serial('/dev/ttyAMA1', 115200)
print(ser1.name)
with open('milestone2.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    for row in csv_reader:
        angle = row[0]
        speed = row[1]
        duration = row[2]
        command_str = "angle: " + angle + " speed: " + speed + " duration: " + duration + "\n"
        print(command_str)
        #ser1.write(command_str)
        ser1.write(bytes(command_str))

ser1.close()
