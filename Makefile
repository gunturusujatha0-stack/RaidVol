
CC = gcc
CFLAGS = -Wall -Wextra -g -D_FILE_OFFSET_BITS=64 -Iinclude
THREAD_FLAGS = -pthread

TARGET = raidvol
TEST_TARGET = test_raid
CONCURRENCY_TARGET = test_concurrency

SRC = src/main.c src/disk.c src/raid.c
TEST_SRC = tests/test_raid.c src/disk.c src/raid.c
CONCURRENCY_SRC = tests/test_concurrency.c src/disk.c src/raid.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_TARGET)

$(CONCURRENCY_TARGET): $(CONCURRENCY_SRC)
	$(CC) $(CFLAGS) $(CONCURRENCY_SRC) $(THREAD_FLAGS) -o $(CONCURRENCY_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

concurrency: $(CONCURRENCY_TARGET)
	./$(CONCURRENCY_TARGET)

check: all test concurrency

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(CONCURRENCY_TARGET) *.o

.PHONY: all test concurrency check clean
