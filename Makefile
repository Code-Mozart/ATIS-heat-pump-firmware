# Config

NAME = heat-pump-firmware
INTERMEDIATE_DIR = output/int
OUTPUT_DIR = output/bin

CPP_VERSION = c++17

# Arguments

MAC_ADDRESS = 00:00:00:00:00:00
API_KEY_FILE_PATH = ./token

# Dependencies

DEPENDENCIES = curl

# Script

source_files := $(shell find . -type f -iname '*.cpp')
object_files := $(patsubst ./src/%,$(INTERMEDIATE_DIR)/%,$(source_files:.cpp=.o))

COMPILER_FLAGS = -Wall -Wextra -Werror -Wpedantic -Iinclude -std=c++11
ARGS = $(MAC_ADDRESS) $(API_KEY_FILE_PATH)

debug:
	@echo All sources: $(source_files)
	@echo All objects: $(object_files)

$(INTERMEDIATE_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@g++ $(COMPILER_FLAGS) -c $< -o $@

compile: clean-intermediate $(object_files)

link: clean-output
	@mkdir -p $(OUTPUT_DIR)
	@g++ $(COMPILER_FLAGS) $(object_files) -o $(OUTPUT_DIR)/$(NAME) -l$(DEPENDENCIES)

build: compile link

run: build
	@$(OUTPUT_DIR)/$(NAME) $(ARGS)

# Clean

clean-intermediate:
	@rm -rf $(INTERMEDIATE_DIR)

clean-output:
	@rm -rf $(OUTPUT_DIR)

clean: clean-intermediate clean-output
