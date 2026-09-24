SOURCE=		$(wildcard *.c)
TARGET=		$(SOURCE:.c=)

all:		$(TARGET)

%:		%.c
	gcc -Wall -g -std=gnu99 -pthread -o $@ $<

clean:
	rm -f $(TARGET)
