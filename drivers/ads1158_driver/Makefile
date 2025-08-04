CC=gcc
SRCS=main.c ads1158.c
OBJS=$(SRCS:.c=.o)
TARGET=test_ads

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)