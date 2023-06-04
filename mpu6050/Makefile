CC = g++
CFLAGS = -lwiringPi
OBJ_NAME = main

DIRS = . ./mpu6050 ./helper ./motorhatlib
# DIRS = test.cpp

CPP_FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
OBJ_FILES = $(patsubst %.cpp, %.o, $(CPP_FILES))
HEADER_FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.h $(dir)/*.hpp))

test:
	$(info )
	$(info CPP_FILES: $(CPP_FILES))
	$(info OBJ_FILES: $(OBJ_FILES))
	$(info HEADER_FILES: $(HEADER_FILES))

$(OBJ_NAME): $(OBJ_FILES)
	$(info )
	$(info Linking the CPU executable:)
	$(CC) -o $@ $(OBJ_FILES) $(CFLAGS)

%.o: %.cpp %.h
	$(info )
	$(info Compiling object file for %.cpp)
	$(CC) -c -o "$@" "$<" $(CFLAGS)

%.hpp:
	$(info )
	$(info %.hpp has changed)
%.h:
	$(info )
	$(info %.h has changed)


clean:
	rm $(OBJ_FILES) $(OBJ_NAME)
