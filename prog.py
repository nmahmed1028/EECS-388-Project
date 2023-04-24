import serial

ser1 = serial.Serial('/dev/ttyAMA1')
ser.baudrate = 115200

file = open('milestone2.csv')

for line in file:
  num  = line.split()
  speed = num[1]
  angle = num[0]
  duration = num[2]
  command_str = "angle: " + angle + "speed: " + speed + "duration: " + duration
  ser1.write(bytes(command_str))
ser1.close()
