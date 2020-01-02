import time
import board
import pulseio
from adafruit_motor import servo


pwm = pulseio.PWMOut(board.A2, duty_cycle=2 ** 15, frequency=50)

# you can change the defaults:
#   range: 0-135
# most servos do 0-180
s = servo.Servo(
    pwm
    # min_pulse=700,
    # max_pulse=2100,
    # actuation_range=180
)
s.angle = 80
time.sleep(0.5)

pwm2 = pulseio.PWMOut(board.A1, frequency=50)
mc = servo.ContinuousServo(pwm2)


mc.throttle = 1
print(">> forward")
time.sleep(2)
mc.throttle = -1
print(">> reverse")
time.sleep(2)
mc.throttle = 0
print(">> stop")
time.sleep(1)

for angle in range(50, 110, 5):  # 0 - 180 degrees, 5 degrees at a time.
    s.angle = angle
    time.sleep(0.1)
