# Uncomment lines below if you have problems with $PATH
#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))

all:
	platformio -f -c vim run

upload:
	platformio -f -c vim run --target upload

upload-n-monitor:
	@$(MAKE) -f $(THIS_FILE) upload
	@$(MAKE) -f $(THIS_FILE) monitor

upload-n-monitor-mac:
	@$(MAKE) -f $(THIS_FILE) upload
	@$(MAKE) -f $(THIS_FILE) monitor-mac

clean:
	platformio -f -c vim run --target clean

program:
	platformio -f -c vim run --target program

uploadfs:
	platformio -f -c vim run --target uploadfs

update:
	platformio -f -c vim update

monitor:
	pio device monitor --port  /dev/ttyUSB0 --baud 115200

monitor-mac:
	pio device monitor --port  /dev/cu.SLAB_USBtoUART --baud 115200
