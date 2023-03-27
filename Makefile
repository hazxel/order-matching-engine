BLD_DIR = build
DPT_DIR = build/.dpt
BIN_DIR = bin
SRC_DIR = src

CPP_SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp")
CPP_OBJ_FILES = $(patsubst $(SRC_DIR)%, $(BLD_DIR)%, $(patsubst %.cpp, %.o, $(CPP_SRC_FILES)))
CPP_DPT_FILES = $(patsubst $(SRC_DIR)%, $(DPT_DIR)%, $(patsubst %.cpp, %.d, $(CPP_SRC_FILES)))

# put targets at the root of folder src!
TARGETS = main
TARGET_BIN_FILES = $(addprefix $(BIN_DIR)/, $(TARGETS))
TARGET_OBJ_FILES = $(patsubst %, $(BLD_DIR)/%.o, $(TARGETS))
SUPPORT_OBJ_FILES = $(filter-out $(TARGET_OBJ_FILES), $(CPP_OBJ_FILES))

CC = clang++
CC_FLAGS = -std=c++20
LDFLAGS =


all: $(TARGET_BIN_FILES) 

$(BIN_DIR)/%: $(BLD_DIR)/%.o $(SUPPORT_OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BLD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(DPT_DIR)/%.d: $(SRC_DIR)/%.cpp 
	mkdir -p $(dir $@); \
	set -e; \
	rm -f $@; \
	$(CC) -MM $(CC_FLAGS) $< > $@.$$$$.dtmp; \
	sed 's,\($(notdir $*)\)\.o[ :]*,$(patsubst $(DPT_DIR)%.d,$(BLD_DIR)%.o,$@) $@ : ,g' < $@.$$$$.dtmp > $@; \
	rm -f $@.$$$$.dtmp

-include $(CPP_DPT_FILES)

.PHONY: clean
clean:
	rm -rf $(BLD_DIR)
	rm -rf $(BIN_DIR)