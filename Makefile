CC=vc
PLATFORM=aos68k
INCLUDE_DIR=/Volumes/workspace/amiga_dev/NDK/NDK3.2R4/Include_H
LIB_DIR=/Volumes/workspace/amiga_dev/NDK/NDK3.2R4/lib
SRC_DIRS=./src
BUILD_DIR=./build_vbcc

all: $(BUILD_DIR)/test $(BUILD_DIR)/testv37

INC_FLAGS=-I$(INCLUDE_DIR)
CFLAGS := $(INC_FLAGS) +$(PLATFORM)
LDFLAGS := +$(PLATFORM) -L$(LIB_DIR) -ldebug -lamiga

SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/test: $(BUILD_DIR)/src/test.o $(BUILD_DIR)/src/mount.o
	$(CC) $(BUILD_DIR)/src/test.o $(BUILD_DIR)/src/mount.o -o $@ $(LDFLAGS)

$(BUILD_DIR)/testv37: $(BUILD_DIR)/src/test.o $(BUILD_DIR)/src/mountv37.o
	$(CC) $(BUILD_DIR)/src/test.o $(BUILD_DIR)/src/mountv37.o -o $@ $(LDFLAGS)

#$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	#$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
