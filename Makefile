TARGET := cronus
CFLAGS := -g -O1 -Wall -Wextra
INCLUDE_DIRS := -Isrc/
MAKE_DEP := -MMD -MP

SRCS := $(shell find src/ -type f -name "*.c")
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(MAKE_DEP) $(INCLUDE_DIRS) -c -o $@ $<

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS) $(DEPS)
