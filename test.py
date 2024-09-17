import time
from python_hue_v2 import Hue

time.sleep(1)  # wait for search

hue = Hue('<BRIDGE_IP_ADDRESS>', '<HUE_API_USERNAME / KEY>')  # create Hue instance

lights = hue.lights

for light in lights:
    print(light.on)
    light.on = True
    light.brightness = 80.0
