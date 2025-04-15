BIN = main

all: $(BIN)

clean:
	rm -f $(BIN) *.o *.swp

main: main.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt
