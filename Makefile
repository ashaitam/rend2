OUT      := project
CXX      := g++
CXXFLAGS := -Wall -std=c++17
PKGS     := gtkmm-4.0
LINKING  := $(shell pkg-config --cflags $(PKGS))
LDLIBS   := $(shell pkg-config --libs $(PKGS))

BUILD_DIR := build

CXXFILES := graphic.cc project.cc gui.cc
OFILES   := $(addprefix $(BUILD_DIR)/, $(CXXFILES:.cc=.o))

.PHONY: all clean tests

all: $(OUT)

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@

$(OUT): $(OFILES)
	@$(CXX) $(CXXFLAGS) $(LINKING) $^ -o $@ $(LDLIBS)

clean:
	@echo "Cleaning project..."
	@rm -rf $(BUILD_DIR) $(OUT)

tests: $(OUT)
	@for test in $$(ls tests); do \
		echo "Running $$test..."; \
		./$(OUT) tests/$$test; \
	done
