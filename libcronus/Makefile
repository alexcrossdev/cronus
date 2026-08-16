TARGET := libcronus.so

CFLAGS := -g -Wall -Wextra -Werror -Wpedantic -fPIC
DFLAGS := -MMD -MP

SRCS := $(shell find src/ -type f -name "*.c" ! -name "*_platform.c")
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

INCS := -Iinclude

.PHONY: all clean bear install

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -shared -o $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) $(DFLAGS) $(INCS) -o $@ $<

clean:
	rm $(TARGET) $(OBJS) $(DEPS)

bear: clean
	bear -- $(MAKE)

install: all
	sudo cp $(TARGET) /usr/lib

-include $(DEPS)
