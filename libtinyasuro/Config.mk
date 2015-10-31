# Library name
LIBRARY_NAME = libtinyasuro

# MCU name
#     The only supported MCU ist the atmega8
MCU = atmega8

# Boot section start
#     The address of the first byte (!) of the boot section.
BOOT_START=0x1E00

# Threshold for low battery warning
LOW_BAT_THRESHOLD=165 # ca. 3.8V

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the 
#     processor frequency. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     The only supported frequency is 8MHz
F_CPU = 8000000

# Baudrate used for IR communication, can be overwritten by config.h
BAUDRATE = 2400
