CC = g++
CFLAGS = -W -Wall 
TARGET = build/examples/usb_test

MODULES = serial udp temp_control common examples
SRC_DIR = $(addprefix src/,$(MODULES))
BUILD_DIR = $(addprefix build/,$(MODULES))

SRC = $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))
INCLUDES = $(addprefix -I,$(SRC_DIR))
LDFLAGS = -lpthread

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $$< -o $$@ $(LDFLAGS)
endef

all : checkdirs $(TARGET)

checkdirs : $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)


clean : 
	@rm -rf $(BUILD_DIR)
$(foreach bdir, $(BUILD_DIR),$(eval $(call make-goal, $(bdir))))
