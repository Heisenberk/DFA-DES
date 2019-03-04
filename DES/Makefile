
# Clément Caumes

# -g -Wall -Wextra a remettre
CFLAGS=-c
EXEC=des
CC=gcc
SRC=app/src
INC=app/inc
TEST=test
BLD=build
OBJ= main.o key_schedule.o inner_function.o errors.o manip_bits.o feistel.o attack.o

PATH_OBJ_WITHOUT_MAIN= $(BLD)/src/key_schedule.o $(BLD)/src/inner_function.o $(BLD)/src/errors.o $(BLD)/src/manip_bits.o $(BLD)/src/feistel.o $(BLD)/src/attack.o
PATH_OBJ= $(BLD)/src/main.o $(PATH_OBJ_WITHOUT_MAIN)
PATH_TEST_OBJ_WITHOUT_MAIN= $(BLD)/test/test_key_schedule.o $(BLD)/test/test_inner_function.o $(BLD)/test/test_manip_bits.o $(BLD)/test/test_feistel.o
PATH_TEST_OBJ= $(BLD)/test/test_des.o $(PATH_TEST_OBJ_WITHOUT_MAIN)

run: compil $(BLD)
	$(BLD)/src/$(EXEC)

compil: $(BLD) $(OBJ)
	$(CC) -o $(BLD)/src/$(EXEC) $(PATH_OBJ)

main.o: build_src key_schedule.o errors.o
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(BLD)/src/main.o

attack.o: build_src key_schedule.o inner_function.o manip_bits.o feistel.o
	$(CC) $(CFLAGS) $(SRC)/attack.c -o $(BLD)/src/attack.o 

key_schedule.o: build_src errors.o manip_bits.o
	$(CC) $(CFLAGS) $(SRC)/key_schedule.c -o $(BLD)/src/key_schedule.o 

manip_bits.o: build_src errors.o 
	$(CC) $(CFLAGS) $(SRC)/manip_bits.c -o $(BLD)/src/manip_bits.o 

inner_function.o: build_src errors.o manip_bits.o
	$(CC) $(CFLAGS) $(SRC)/inner_function.c -o $(BLD)/src/inner_function.o 

feistel.o: build_src errors.o manip_bits.o
	$(CC) $(CFLAGS) $(SRC)/feistel.c -o $(BLD)/src/feistel.o 

errors.o: build_src
	$(CC) $(CFLAGS) $(SRC)/errors.c -o $(BLD)/src/errors.o

test: clean $(BLD) $(OBJ) test_des.o test_key_schedule.o test_inner_function.o test_manip_bits.o test_feistel.o
	$(CC) -o $(BLD)/test/test_des $(PATH_TEST_OBJ) $(PATH_OBJ_WITHOUT_MAIN) -lcunit
	$(BLD)/test/test_des

test_des.o: build_test test_key_schedule.o test_inner_function.o test_manip_bits.o
	$(CC) $(CFLAGS) $(TEST)/src/test_des.c -o $(BLD)/test/test_des.o

test_key_schedule.o: build_test errors.o key_schedule.o manip_bits.o
	$(CC) $(CFLAGS) $(TEST)/src/test_key_schedule.c -o $(BLD)/test/test_key_schedule.o

test_manip_bits.o: build_test errors.o key_schedule.o manip_bits.o
	$(CC) $(CFLAGS) $(TEST)/src/test_manip_bits.c -o $(BLD)/test/test_manip_bits.o

test_inner_function.o: build_test errors.o inner_function.o manip_bits.o
	$(CC) $(CFLAGS) $(TEST)/src/test_inner_function.c -o $(BLD)/test/test_inner_function.o

test_feistel.o: build_test 
	$(CC) $(CFLAGS) $(TEST)/src/test_feistel.c -o $(BLD)/test/test_feistel.o

build: 
	mkdir -p $(BLD)/
	
build_src: build
	mkdir -p $(BLD)/src/
	
build_test: build
	mkdir -p $(BLD)/test/

doxygen:
	doxygen doc/Doxyfile
	firefox doc/html/files.html

clean: 
	rm -rf build/
	rm -rf doc/html/
	rm -rf doc/latex/

install: 
	sudo apt-get install doxygen
	sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev
