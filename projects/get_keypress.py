#!/usr/bin/python3
from tkinter import *
import serial

def on_button_press(event):
	key = event.char
	ser= serial.Serial("/dev/ttyUSB0", 2400)
	ser.write(key.encode())
	print(key)
    

root = Tk()

root.bind_all("<Key>",on_button_press)
root.mainloop()
