#!/usr/bin/env python3

from gpiozero import PWMOutputDevice, DigitalOutputDevice
from time import sleep

SPEED = 0.50

# 모터 A (GPIO 27, 17번 핀을 사용하는 모터)
motor_a_forward = DigitalOutputDevice(17)
motor_a_backward = DigitalOutputDevice(27)
motor_a_pwm = PWMOutputDevice(12, frequency=1000)

# 모터 B (GPIO 24, 23번 핀을 사용하는 모터)
motor_b_forward = DigitalOutputDevice(24)
motor_b_backward = DigitalOutputDevice(23)
motor_b_pwm = PWMOutputDevice(13, frequency=1000)

def motor_a_control(speed, direction):
    if direction == 'forward':
        motor_a_forward.on()
        motor_a_backward.off()
    elif direction == 'backward':
        motor_a_forward.off()
        motor_a_backward.on()
    motor_a_pwm.value = speed

def motor_b_control(speed, direction):
    if direction == 'forward':
        motor_b_forward.on()
        motor_b_backward.off()
    elif direction == 'backward':
        motor_b_forward.off()
        motor_b_backward.on()
    motor_b_pwm.value = speed

def rotate_left(speed):
    motor_a_control(speed, 'backward')  # 왼쪽 바퀴 후진
    motor_b_control(speed, 'forward')   # 오른쪽 바퀴 전진
    motor_a_pwm.value = speed           # 왼쪽 바퀴 속도 제어
    motor_b_pwm.value = speed           # 오른쪽 바퀴 속도 제어

def rotate_right(speed):
    motor_a_control(speed, 'forward')   # 왼쪽 바퀴 전진
    motor_b_control(speed, 'backward')  # 오른쪽 바퀴 후진
    motor_a_pwm.value = speed           # 왼쪽 바퀴 속도 제어
    motor_b_pwm.value = speed           # 오른쪽 바퀴 속도 제어

def stop_all_motors():
    motor_a_forward.off()
    motor_a_backward.off()
    motor_a_pwm.off()
    motor_b_forward.off()
    motor_b_backward.off()
    motor_b_pwm.off()

try:
    while True:
        # 모터 A와 B를 전진 방향으로 1/3 속도로 제어
        motor_a_control(SPEED, 'forward')
        motor_b_control(SPEED, 'forward')
        sleep(2)

        rotate_left(SPEED)
        sleep(4)

        # 모터 A와 B를 후진 방향으로 1/3 속도로 제어
        motor_a_control(SPEED, 'backward')
        motor_b_control(SPEED, 'backward')
        sleep(2)

        rotate_right(SPEED)
        sleep(4)

except KeyboardInterrupt:
    stop_all_motors()