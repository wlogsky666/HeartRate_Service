import serial
import matplotlib.pyplot as plt


time = 1
lastH = 0
lasttimeH = 0

plt.ion()
plt.show()


with serial.Serial('/dev/ttyACM0', 9600, timeout=2) as s:
	while True:
		line = s.readline().decode('utf-8') 
		time += 1

		if line.startswith('Heartrate : '):
			line = line[len('Heartrate : '):]

			h = float(line.split(' ')[0])
			if lastH != 0 :
				plt.plot([lasttimeH, time], [lastH, h], color='r')
				print([lasttimeH, time], [lastH, h])
			
			lasttimeH = time
			lastH = h

			plt.pause(1e-6)

	s.close()

plt.pause(300)
