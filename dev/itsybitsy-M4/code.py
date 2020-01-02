import time
import board
# import digitalio
import pulseio
# from adafruit_motor import servo
# import busio
import neopixel
import microcontroller


# import adafruit_rtttl

# Trinket M0
# ----------------------------------
# ATSAMD21E18 32-bit Cortex M0+
# 256KB Flash
# 32 KB RAM
# 48 MHz 32 bit processor
# ---------------------------------
# https://circuitpython.org/libraries
# https://learn.adafruit.com/adafruit-trinket-m0-circuitpython-arduino/circuitpython-servo
# https://learn.adafruit.com/adafruit-trinket-m0-circuitpython-arduino/circuitpython-uart-serial-2
# https://circuitpython.readthedocs.io/projects/rtttl/en/latest/examples.html

# led = digitalio.DigitalInOut(board.D13)
# led.direction = digitalio.Direction.OUTPUT

# uart = busio.UART(board.TX, board.RX, baudrate=115200, timeout=1)

# create a PWMOut object on Pin A2.
# pwm = pulseio.PWMOut(board.A2, duty_cycle=2 ** 15, frequency=50)

# pwm2 = pulseio.PWMOut(board.A1, duty_cycle=2 ** 15, frequency=50)

# enable = digitalio.DigitalInOut(board.SPEAKER_ENABLE)
# enable.switch_to_output(value=True)

# adafruit_rtttl.play(board.A0, "itchy:d=8,o=6,b=160:c,a5,4p,c,a,4p,c,a5,c,a5," + \
#     "c,a,4p,p,c,d,e,p,e,f,g,4p,d,c,4d,f,4a#,4a,2c7")


# mnpw = 650
# mxpw = 2400
# Create a servo object, my_servo.
# my_servo = servo.Servo(pwm, min_pulse=mnpw, max_pulse=mxpw,actuation_range=180)
# my_servo = servo.Servo(pwm)
# my_servo.actuation_range = 135
# my_servo2 = servo.Servo(pwm2, min_pulse=mnpw, max_pulse=mxpw,actuation_range=180)
# my_servo2 = servo.Servo(pwm2)




class Piezo:
    def __init__(self, pin):
        self.piezo = pulseio.PWMOut(
            pin,
            duty_cycle=0,
            frequency=440,
            variable_frequency=True
        )

    def tone(self, f, duration=0.25):
        self.piezo.frequency = f
        self.piezo.duty_cycle = 65536 // 2  # On 50%
        time.sleep(duration)  # On for 1/4 second
        self.piezo.duty_cycle = 0  # Off
        # time.sleep(0.05)  # Pause between notes

class Pixels:
    RED = (255, 0, 0)
    YELLOW = (255, 150, 0)
    GREEN = (0, 255, 0)
    CYAN = (0, 255, 255)
    BLUE = (0, 0, 255)
    PURPLE = (180, 0, 255)

    def __init__(self, pin, num_pixels=1, bright=0.3):
        self.pixels = neopixel.NeoPixel(
            pin,
            num_pixels,
            brightness=bright,
            auto_write=False
        )

    def fill(self, color):
        """
        color - (r,g,b) with values of 0-255
        """
        self.pixels.fill(color)
        self.pixels.show()

p = Pixels(board.A0, bright=0.1)
pie = Piezo(board.A2)

# Camera ---------------
import adafruit_amg88xx
i2c = busio.I2C(board.SCL, board.SDA)
amg = adafruit_amg88xx.AMG88XX(i2c)

while True:
    print("Loop, processor temp [F]:", microcontroller.cpu.temperature*(9/5)+32)
    print("cpu", (microcontroller.cpu))
    # led.direction = digitalio.Direction.INPUT
    # led.value = True
    # data = uart.read(32)  # read up to 32 bytes
    # print(">>", data)  # this is a bytearray type
    # # led.direction = digitalio.Direction.OUTPUT
    # led.value = False
    # uart.write(b"hello there 1.234\n")
    # # for angle in range(0, 135, 5):  # 0 - 180 degrees, 5 degrees at a time.
    # angle = 180
    # my_servo.angle = angle
    # my_servo2.angle = angle
    # time.sleep(2)
    # # for angle in range(135, 0, -5): # 180 - 0 degrees, 5 degrees at a time.
    # angle = 0
    # my_servo.angle = angle
    # my_servo2.angle = angle

    for c in [Pixels.RED, Pixels.YELLOW, Pixels.GREEN, Pixels.CYAN, Pixels.BLUE, Pixels.PURPLE]:
        p.fill(c)
        time.sleep(1)

    for f in (262, 294, 330, 349, 392, 440, 494, 523):
        pie.tone(f)
        time.sleep(0.05)
