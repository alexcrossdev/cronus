TARGET := cronus

LIBCRONUS_DIR := ../libcronus

CFLAGS := -g -Wall -Wextra -Werror -Wpedantic
DFLAGS := -MMD -MP

INCS := \
	-Isrc \
	-I$(LIBCRONUS_DIR)/include

LDFLAGS := \
	-L$(LIBCRONUS_DIR) \
	-Wl,-rpath,'$$ORIGIN/../libcronus'

LDLIBS := -lcronus

SRCS := $(shell find src -type f -name "*.c")
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS) $(LIBCRONUS_DIR)/libcronus.so
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(DFLAGS) $(INCS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

.PHONY: bear
bear: clean
	bear -- $(MAKE)

-include $(DEPS)
