TEST_DIR = test
TARGET = main.c
PROGRAM_NAME = prog

all: build prepare run

prepare:
	./prepare.sh $(TEST_DIR)

build:
	gcc $(TARGET) -o $(PROGRAM_NAME)

clean:
	rm -rf $(TEST_DIR)
	rm -f $(PROGRAM_NAME)

run:
	./$(PROGRAM_NAME) $(TEST_DIR)
