TARGET = main
LINK_LIBS += -lpmem
SRC_FOLDER = ./src
CC = gcc
CFLAGS += -g -O2

all: $(TARGET)


$(TARGET): $(SRC_FOLDER)/*.c  $(SRC_FOLDER)/*.h
	$(CC) $(CFLAGS) $(filter $(wildcard **/*.c), $^) $(LINK_LIBS) -o $(TARGET)

clean: $(TARGET)
	rm $(TARGET)