TARGET := libcronus.so

CFLAGS := -g -Wall -Wextra -Werror -Wpedantic -fPIC
DFLAGS := -MMD -MP

SRCS := $(shell find src/ -type f -name "*.c")
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

INCS := -Iinclude

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -shared -o $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) $(DFLAGS) $(INCS) -o $@ $<

.PHONY: clean
clean:
	rm $(TARGET) $(OBJS) $(DEPS)

.PHONY: bear
bear: clean
	bear -- $(MAKE)

-include $(DEPS)
