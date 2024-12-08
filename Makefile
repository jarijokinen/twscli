TARGET=twscli

CC=gcc
RM=rm
CFLAGS=-std=c17 -Wall -Wextra -Wpedantic -pthread
OBJS=\
	build/main.o \
	build/twsapi/connection.o \
	build/twsapi/message.o

build/%.o: src/%.c
	mkdir -p build/twsapi
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@

all: $(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET)
