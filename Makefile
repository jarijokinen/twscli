TARGET=twscli

CC=gcc
RM=rm
CFLAGS=-std=c17 -Wall -Wextra -Wpedantic -pthread
OBJS=main.o

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@

all: $(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET)
