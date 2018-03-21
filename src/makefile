OBJS = main.o
CC = g++
CFLAGS = -Wall -Wextra -std=c99 --pedantic-errors

.PHONY: all

all: $(OBJS)
	 $(CC) $(LFLAGS) $(OBJS) -o SlackRC


clean:
	rm -f $(OBJS)
	rm -f SlackRC