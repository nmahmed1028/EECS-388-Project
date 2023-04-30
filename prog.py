import serial
import time
import csv

ser1 = serial.Serial('/dev/ttyAMA1', 115200) #initializes ser1 as UART channel that program will use, 115200 is the baudrate

with open('milestone2.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',') #reads from csv file, separating elements by comma
    for row in csv_reader:
        #first value in line is for angle, second is for speed, third is for duration
        angle = row[0] 
        speed = row[1]
        duration = row[2]
        command_str = "angle: " + str(angle) + " speed: " + str(speed) + " duration: " + str(duration) + "\n" #output string, outputs each line's angle, speed, duration
        ser1.write(bytes(command_str)) #writes command_str to channel
        time.sleep(2) #delay between each line
ser1.close()
