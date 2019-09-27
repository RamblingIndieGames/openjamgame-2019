MKDIR_P ?= mkdir -p
CC ?= gcc
CFLAGS ?= $(shell pkg-config sdl2_image sdl2_mixer --cflags) -Os
LDFLAGS ?= $(shell pkg-config sdl2_image sdl2_mixer --libs) -Wl,-headerpad_max_install_names
TARGET_EXEC ?= openjamgame
BUILD_DIR ?= ./bin
SRC_DIR ?= ./src
SOURCES := $(shell find $(SRC_DIR) -name *.c)
OBJECTS := $(SOURCES:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJECTS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
EXT_FLAGS ?= $(INC_FLAGS) -MMD -MP -D DEBUG
# EXT_FLAGS ?= $(INC_FLAGS) -MMD -MP -D PRODUCTION
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(EXT_FLAGS) $(CFLAGS) -c $< -o $@
.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)
-include $(DEPS)
