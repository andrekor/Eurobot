import serial
import time

from pylab import *
#import matplotlib.pyplot as pl
 
#ser = serial.Serial('/dev/ttyUSB1', 9600)
x = linspace(0, 5, 10)
y = x ** 2

figure();
plot(x, y, 'r');
xlabel('x');
ylabel('y');
title('sdf');
show();

#axis([0 300 0 200]) 

#from pylab import *
#timeMeasure = []#List of timestamps 
#valueMeasure = []#List of values measured at the time in timeMeasure
#ser = serial.Serial('/dev/ttyUSB1', 9600)
#print time.time()	
#
#a = 0;
#plotTime = 10

#stop = time.time()+plotTime
#startTime = time.time();
#ser.flush() #flush the serial
#while time.time() < stop:
	#var = ser.read();
	#if (var == '1') or (var == '0'):
		#timeMeasure.append(time.time()); #time
		#valueMeasure.append(var); #1 or 0
		#print var	

#for i in range(len(timeMeasure)):
	#print ((startTime-timeMeasure[i]), valueMeasure[i])