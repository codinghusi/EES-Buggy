# Dieser Code ist im Abschlussprojekt des Moduls "Einführung in Embedded Software" entstanden.
# Hannah Lehnen, Timur Gönül und Gerrit Weiermann
# Gruppe 18, Slot D

CC = g++
CFLAGS = -g -lwiringPi -pthread
OBJ_NAME = main

OBJ_DIR = ./obj
EXCLUDE = test-main.cpp


CPP_FILES = $(filter-out %$(EXCLUDE), $(shell find . -name '*.cpp'))
HPP_FILES = $(shell find . -name '*.hpp')
H_FILES = $(shell find . -name '*.h')
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
	@$(CC) -c -o "$@" "$<" $(CFLAGS)
	$(info compiling $<)

clean:
	rm -rf obj
	mkdir obj

debug:
	$(info )
	$(info CPP_FILES: $(CPP_FILES))
	$(info OBJ_FILES: $(OBJ_FILES))
	$(info HEADER_FILES: $(H_FILES))

run:
	./$(OBJ_NAME)

shit:
	make
	make run

gyro-test:
	make shit -C mpu6050
