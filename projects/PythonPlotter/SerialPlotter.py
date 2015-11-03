#######
# The Serial plotter is likely to be quite slow, unfortunately. Don't expect data at rates of more than 20Hz to be plotted...
# The expected data format is "a,dataLeft,dataRight\n", for example "a,123,456\n"
#######


# Example:
# To plot the data from the two photodiodes in the front, execute the following function(or similar) on your Asuro at about 10-20Hz:
"""
void printLineSensors(void)
{
	unsigned short SerDelay = 5;
	LineData(linData);
	SerPrint("a,");
	Msleep(SerDelay);
	PrintInt(linData[LEFT]);
	Msleep(SerDelay);
	SerPrint(",");
	Msleep(SerDelay);
	PrintInt(linData[RIGHT]);
	Msleep(SerDelay);
	SerPrint("\n");
}
"""


import time
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation

### PARAMETERS ###
# figure configuration
plot_title = "Serial data live plot"
plot_xlabel = "step"
plot_ylabel = "data"
line_format = "b"
line_format_2 = "r"
backview = 100
preview = 20
y_max = 1100
y_min = -1100

# serial port
serial_port = "/dev/ttyUSB0"
serial_baudrate = 2400
serial_initWait = 2

# data format specifics
headers = ['a', 'b', 'l','p']
separator = ','

### SET UP AND OPEN THE SERIAL PORT ###
ser = serial.Serial(serial_port, serial_baudrate, timeout=1)
time.sleep(serial_initWait)


### SETTTING UP OF THE FIGURE ####
fig1 = plt.figure(figsize=(4,4))
plt.ylim(y_min, y_max)
plt.xlabel(plot_xlabel)
plt.ylabel(plot_ylabel)
plt.title(plot_title)
plt.xlim(-1*backview, preview)
ax = plt.axes()
line_1, = ax.plot([], [], line_format)
## line two
line_2, = ax.plot([], [], line_format_2)

x = [0]
y = [0]
z = [0]


def init():
	line_1.set_data([],[])
	line_2.set_data([],[])
	return line_1, line_2


def anim (num):
	#print "hello"
	### DO WHAT IT TAKES TO GET THE NEXT DATAPOINT HERE - I.E. SERIAL INPUT ###
	ser.flushInput()
	header_found = False
	while (header_found == False):
		header = ser.read()
		if (header in headers):
			header_found = True

	ser.read() # erase the leading comma from the buffer
	msg_aux = ser.readline()
	msg = msg_aux.split(separator) # make sure to use the correct separator for split()!

	# extract x-value (i.e. time)
	x_new = float(num)

	if len(msg) == 2:
		try:
			# extract y
			y_new = float(msg[0])
			# extract z
			z_new = float(msg[1])
		except:
			y_new = y[num-1];
			z_new = z[num-1];
			print msg
	else:
		print "errenous data received"
		y_new = y[num-1];
		z_new = z[num-1];


	####################################################################
	# append the new data to the corresponding lists
	x.append(x_new)
	y.append(y_new)
	z.append(z_new)
	# create the updated line object
	line_1.set_data(x,y)
	line_2.set_data(x,z)
	# update the x-axis to make sure the new data is within view
	plt.xlim(x_new - backview, x_new + preview)
	return line_1, line_2


# Call the animator and show the figure
try:
	line_ani = animation.FuncAnimation(fig1, anim, init_func=init, interval = 50, blit=True, repeat = False)
	plt.show()
except:
	print "Animation aborted"

print "Plotting complete dataset..."
plt.plot(x,y,x,z)
plt.show()
