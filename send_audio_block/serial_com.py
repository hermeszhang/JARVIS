import serial
import matplotlib.pyplot as plotter

ser = serial.Serial('/dev/ttyACM1', 57600)
#s = numpy.ones(10)
s = 0
ampl = []
ctr = 0;
while ctr < 3000:
	#read_serial = ser.readline()
	s = float(ser.readline())
	#print s
	ampl.append(s)
	ctr += 1
ser.close()
#Display data
print ampl
plotter.plot(ampl)
plotter.show()
#Write to file
file = open('Speech.txt', 'w')
file.write(str(ampl))
file.close()
