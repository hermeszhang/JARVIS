import serial

ser = serial.Serial('/dev/ttyACM1', 115200)
s = [0]
while True:
	#read_serial = ser.readline()
	s[0] = float(ser.readline())
	print s
	#print read_serial
ser.close()
