CC = g++
CFLAGS = -W -Wall
serial_test = build/examples/serial_test
udp_test = build/examples/udp_test
comm_test = build/examples/comm_test
TARGET = build/cool_vibe

BUILD_ROOT = ./build

MODULES = serial udp temp_control common examples core
SRC_DIR = $(addprefix src/,$(MODULES))
BUILD_DIR = $(addprefix build/,$(MODULES))

SRC_1 = $(foreach sdir, $(SRC_DIR),$(wildcard $(sdir)/*serial*.cpp))
OBJ_1 = $(patsubst src/%.cpp,build/%.o,$(SRC_1))

SRC_2 = $(foreach sdir, $(SRC_DIR),$(wildcard $(sdir)/*udp*.cpp))
OBJ_2 = $(patsubst src/%.cpp, build/%.o,$(SRC_2))

SRC_3 := $(foreach sdir, $(SRC_DIR),$(wildcard $(sdir)/*.cpp))
SRC_3 := $(filter-out src/examples/udp_test.cpp, $(SRC_3))
SRC_3 := $(filter-out src/examples/serial_test.cpp,$(SRC_3))

OBJ_3 = $(patsubst src/%.cpp,build/%.o,$(SRC_3))


SRC := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
SRC := $(filter-out $(wildcard src/examples/*.cpp),$(SRC))
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))


INCLUDES = $(addprefix -I,$(SRC_DIR))
LDFLAGS = -lwiringPi -lpthread

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $$< -o $$@ $(LDFLAGS)
endef





all : checkdirs $(TARGET)

checkdirs : $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

$(serial_test) : $(OBJ_1)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
$(udp_test) : $(OBJ_2)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
$(comm_test) : $(OBJ_3)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean : 
	@rm -rf $(BUILD_ROOT)

rebuild : clean all

$(foreach bdir, $(BUILD_DIR),$(eval $(call make-goal, $(bdir))))
