# Fully Qualified Board Name
fqbn = esp32:esp32:uPesy_wroom

# USB serial port 
usb_port = /dev/ttyUSB0

# Debug serial port
serial_port = 9600

.PHONY: compile upload info monitor runall

compile:
	rm -r bin/*
	arduino-cli compile --fqbn $(fqbn) --build-path ./bin hue-motion-light

upload:
	arduino-cli upload -p $(usb_port) --fqbn $(fqbn) --input-dir ./bin 

info:
	arduino-cli board list

monitor:
	screen $(usb_port) $(serial_port)

test:
	make compile && make upload && make monitor

