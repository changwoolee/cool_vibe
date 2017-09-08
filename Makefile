CC = g++
CFLAGS = -W -Wall
serial_test = build/examples/serial_test
udp_test = build/examples/udp_test

MODULES = serial udp temp_control common examples
SRC_DIR = $(addprefix src/,$(MODULES))
BUILD_DIR = $(addprefix build/,$(MODULES))

SRC_1 = $(foreach sdir, $(SRC_DIR),$(wildcard $(sdir)/*serial*.cpp))
OBJ_1 = $(patsubst src/%.cpp,build/%.o,$(SRC_1))

SRC_2 = $(foreach sdir, $(SRC_DIR),$(wildcard $(sdir)/*udp*.cpp))
OBJ_2 = $(patsubst src/%.cpp, build/%.o,$(SRC_2))

SRC = $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))
INCLUDES = $(addprefix -I,$(SRC_DIR))
LDFLAGS = -lpthread

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $$< -o $$@ $(LDFLAGS)
endef





all : checkdirs $(serial_test) $(udp_test)

checkdirs : $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

$(serial_test) : $(OBJ_1)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
$(udp_test) : $(OBJ_2)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean : 
	@rm -rf $(BUILD_DIR)

rebuild : clean all

$(foreach bdir, $(BUILD_DIR),$(eval $(call make-goal, $(bdir))))
