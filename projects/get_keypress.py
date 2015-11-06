#!/usr/bin/python3
import serial
from tkinter import *

Labels = []

def on_button_press(event):
	global Labels

	key = event.char
	ser= serial.Serial("/dev/ttyUSB0", 2400)
	ser.write(key.encode())
	print(key)

	for lab in Labels:
		lab["relief"]="flat"
	
	if key == "w":
		Labels[0]["relief"] = "ridge"
	elif key == "s":
		Labels[1]["relief"] = "ridge"
	elif key == "a":
		Labels[2]["relief"] = "ridge"
	elif key == "d":
		Labels[3]["relief"] = "ridge"
	elif key != "h" and key != "p":
		messagebox.showwarning("Achtung!","Taste nicht belegt")

root = Tk()
root.title("Steuerung")

root.bind_all("<Key>",on_button_press)

w_Label = Label(root,text="w",font=("Arial", 100))
w_Label.pack(side="top")
s_Label = Label(root,text="s",font=("Arial", 100))
s_Label.pack(side="bottom")
a_Label = Label(root,text="a",font=("Arial", 100))
a_Label.pack(side="left")
d_Label = Label(root,text="d",font=("Arial", 100))
d_Label.pack(side="right")
Labels = [w_Label,s_Label,a_Label,d_Label]

root.mainloop()


