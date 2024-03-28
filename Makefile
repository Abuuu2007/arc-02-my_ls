CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = my_ls

all: $(TARGET)

$(TARGET): my_ls.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)
    
.PHONY: all clean