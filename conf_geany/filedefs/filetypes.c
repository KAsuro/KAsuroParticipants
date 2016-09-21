[build-menu]
FT_00_LB=_Build
FT_00_CM=make all
FT_00_WD=
FT_01_LB=Flash
FT_01_CM=make all && mv main.hex flash.hex && make clean && /bin/con_flash /dev/ttyUSB0 flash.hex
FT_01_WD=
FT_02_LB=_Clean
FT_02_CM=make clean
FT_02_WD=
