#!/usr/bin/env python

from smc import SMC
import time


port = '/dev/serial/by-id/usb-Pololu_Corporation_Pololu_Simple_Motor_Controller_18v7_50FF-6D06-7085-5652-2323-2267-if00'
mc = SMC(port, 115200)
# open serial port and exit safe mode
mc.init()
fw = mc.getFirmware()
print("===================================")
print("Simple Motor Controller")
print(" Firmware:", fw[1])
print(" ID:", fw[0])
print("")

for x in [650, 600, 650, 700, 800, 1000, 1000, 1000, 500, 0]:
    mc.speed(x)
    time.sleep(1)
    print(x)

for x in [-400, -1000, -100]:
    mc.speed(x)
    time.sleep(1)
    print(x)

for x in [-500, -400, -300 -100]:
    mc.speed(x)
    time.sleep(1)
    print(x)

# for x in range(1000, 700, -10):
#     mc.speed(x)
#     time.sleep(0.5)
#     print(x)

print("<<< power down >>>")
mc.speed(0)
time.sleep(3)

# # drive using 12b mode
# mc.speed(1000)
# time.sleep(3)
# mc.speed(-1000)
# time.sleep(3)
#
# # drive using 7b mode
# mc.speed7b(100)
# time.sleep(3)
# mc.speed7b(-100)
# time.sleep(3)

# and stop motor
mc.stop()
