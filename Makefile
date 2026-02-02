CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC = src/main.c src/motor.c src/sensor.c src/hal.c src/pid.c
OUT = robot

all: $(OUT)

$(OUT):
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	del $(OUT).exe
