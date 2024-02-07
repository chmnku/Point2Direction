#!/usr/bin/env python3

from gpiozero import TonalBuzzer
from gpiozero.tones import Tone
from time import sleep

b = TonalBuzzer(25)

melody = [
    Tone(392.00),  # G4
    Tone(523.25),  # C5
    Tone(493.88),  # B4
    Tone(440.00),  # A4
    Tone(392.00)   # G4
]

for note in melody:
    b.play(note)
    sleep(0.25)
    b.stop()

b.stop()
