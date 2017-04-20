import serial
import matplotlib.pyplot as plt

time = 0
lastH = 0
lasttime = 0

plt.ion()
plt.show()

with serial.Serial('COM9', 9600, timeout=2) as s:
	while time < 2000 :
		line = s.readline().decode('utf-8') 
		time += 1

		if line.startswith('Heartrate : '): 
			h = float(line[len('Heartrate : '):].split(' ')[0])
			if lastH != 0 :
				plt.plot([lasttime, time], [lastH, h], color='r')
			lasttime = time
			lastH = h

		if time % 200 == 0 :
			plt.pause(1e-15)

	s.close()

plt.pause(100)
