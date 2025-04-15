BIN = main

all: $(BIN)

clean:
	rm -f $(BIN) *.o *.swp

main: main.o
	gcc -Wall -pthread -o $@ $^ -lpigiod_if2 -lrt
