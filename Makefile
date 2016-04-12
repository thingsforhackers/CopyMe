ARDUINO_DIR = $(ARDUINO_IDE)
BOARD_TAG = atmega328_1mhz_4800baud
ARDUINO_PORT = /dev/ttyUSB0
#ARDUINO_PORT = /dev/ttyACM0
ARDUINO_LIBS =
AVR_TOOLS_PATH = $(HOME)/opt/avr/bin
ARDUINO_ETC_PATH = $(ARDUINO_DIR)/hardware/tools/avr/etc
ARDUINO_OPT_LIB_PATH=../../arduino/libs
include Arduino.mk
