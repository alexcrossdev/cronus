TARGET := cronus

LIBCRONUS_DIR := ../libcronus

CFLAGS := -Wall -Wextra -Werror -Wpedantic
DFLAGS := -MMD -MP

INCS := \
	-Isrc \
	-I$(LIBCRONUS_DIR)/include

SRCS := $(shell find src -type f -name "*.c")
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

LDLIBS := -lcronus -lcrypto

.PHONY: all dev release clean bear install

all: dev

dev: CFLAGS += -g
dev: LDFLAGS += \
	-L$(LIBCRONUS_DIR) \
	-Wl,-rpath,'$$ORIGIN/../libcronus'
dev: $(TARGET)

release: CFLAGS += -O2
release: LDFLAGS := -L/usr/lib
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(DFLAGS) $(INCS) -c -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

bear: clean
	bear -- $(MAKE) dev

install: release
	sudo cp cronus /usr/bin

-include $(DEPS)

