CC := gcc

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
TARGET_NAME := jogo

SOURCES := $(wildcard $(SRC_DIR)/*.c)
HEADERS := $(wildcard $(INC_DIR)/*.h)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

ifeq ($(OS),Windows_NT)
	TARGET := $(BUILD_DIR)/$(TARGET_NAME).exe
	ifneq ($(wildcard raylib/src/raylib.h),)
		RAYLIB_PATH ?= raylib/src
	else
		RAYLIB_PATH ?= C:/raylib/raylib
	endif
	RAYLIB_INCLUDE_PATH ?= $(RAYLIB_PATH)
	RAYLIB_LIBRARY_PATH ?= $(RAYLIB_PATH)
	ifeq ($(wildcard $(RAYLIB_INCLUDE_PATH)/raylib.h),)
		RAYLIB_INCLUDE_PATH := $(RAYLIB_PATH)/include
	endif
	ifeq ($(wildcard $(RAYLIB_LIBRARY_PATH)/libraylib.a),)
		RAYLIB_LIBRARY_PATH := $(RAYLIB_PATH)/lib
	endif
	CFLAGS += -std=c99 -Wall -Wextra -I$(INC_DIR) -I$(RAYLIB_INCLUDE_PATH)
	LDFLAGS += -L$(RAYLIB_LIBRARY_PATH) -lraylib -lopengl32 -lgdi32 -lwinmm
	CREATE_BUILD = powershell -NoProfile -Command "New-Item -ItemType Directory -Force '$(BUILD_DIR)' | Out-Null"
	CLEAN_BUILD = powershell -NoProfile -Command "if (Test-Path '$(BUILD_DIR)') { Remove-Item -Recurse -Force '$(BUILD_DIR)' }"
	RUN_TARGET = $(TARGET)
else
	TARGET := $(BUILD_DIR)/$(TARGET_NAME)
	CFLAGS += -std=c99 -Wall -Wextra -I$(INC_DIR)
	LDFLAGS += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	CREATE_BUILD = mkdir -p $(BUILD_DIR)
	CLEAN_BUILD = rm -rf $(BUILD_DIR)
	RUN_TARGET = ./$(TARGET)
endif

.PHONY: all run clean dirs

all: dirs $(TARGET)

dirs:
	$(CREATE_BUILD)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	$(RUN_TARGET)

clean:
	$(CLEAN_BUILD)
