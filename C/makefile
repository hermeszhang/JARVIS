CC=gcc
CFLAGS= -Wall -g
DEPS = xcorr.h schur.h
OBJ = test.c xcorr.c schur.c
TARGET = test

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	$(RM) $(TARGET)
