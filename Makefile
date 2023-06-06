CC = g++
CFLAGS = -lwiringPi -pthread -ipo
OBJ_NAME = main

DIRS = . ./mpu6050 ./hcsr04 ./motorhatlib ./helper ./helper/angle_difference ./helper/pid ./helper/timer
OBJ_DIR = ./obj
EXCLUDE = test-main.cpp #doesn't work...

CPP_FILES = $(foreach dir, $(DIRS), $(filter-out $(EXCLUDE), $(wildcard $(dir)/*.cpp)))
HPP_FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.hpp))
H_FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.h))
OBJ_FILES = $(patsubst ./%.cpp, $(OBJ_DIR)/%.o, $(CPP_FILES))
OBJ_DIRS = $(foreach dir, $(DIRS), $(patsubst ./%, $(OBJ_DIR)/%/, $(dir)))

all: $(OBJ_NAME)
	$(info )
	$(info To run the application use 'make run'.)
	$(info If you are lazy you can make and run using 'make shit')
	$(info )

$(OBJ_NAME): $(OBJ_FILES)
	$(CC) -o $@ $(OBJ_FILES) $(CFLAGS)
	$(info )
	$(info Linking as ./$(OBJ_NAME))

$(OBJ_DIR)/%.o: %.cpp $(HPP_FILES) $(findstring %.h, $(H_FILES))
	@mkdir -p $(dir $@)
	$(CC) -c -o "$@" "$<" $(CFLAGS)

clean:
	rm -f $(OBJ_FILES) $(OBJ_NAME)

debug:
	$(info )
	$(info CPP_FILES: $(CPP_FILES))
	$(info OBJ_FILES: $(OBJ_FILES))
	$(info HEADER_FILES: $(HEADER_FILES))
	$(info OBJ_DIRS: $(OBJ_DIRS))

run:
	./$(OBJ_NAME)

shit:
	make
	make run
