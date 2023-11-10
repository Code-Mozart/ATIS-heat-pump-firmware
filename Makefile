# Config

NAME = heat-pump
INTERMEDIATE_DIR = output/int
OUTPUT_DIR = output/bin

CPP_VERSION = c++17

# Dependencies

DEPENDENCIES = curl

# Script

source_files := $(shell find . -type f -iname '*.cpp')
object_files := $(patsubst ./src/%,$(INTERMEDIATE_DIR)/%,$(source_files:.cpp=.o))

COMPILER_FLAGS = -std=$(CPP_VERSION) -stdlib=libc++ -Wall -Wextra -Werror -Wpedantic

debug:
	@echo All sources: $(source_files)
	@echo All objects: $(object_files)

$(INTERMEDIATE_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@clang++ $(COMPILER_FLAGS) -c $< -o $@

compile: clean-intermediate $(object_files)

link: clean-output
	@mkdir -p $(OUTPUT_DIR)
	@clang++ $(COMPILER_FLAGS) $(object_files) -o $(OUTPUT_DIR)/$(NAME) -l$(DEPENDENCIES)

build: compile link

run: build
	@$(OUTPUT_DIR)/$(NAME)

# Clean

clean-intermediate:
	@rm -rf $(INTERMEDIATE_DIR)

clean-output:
	@rm -rf $(OUTPUT_DIR)

clean: clean-intermediate clean-output
