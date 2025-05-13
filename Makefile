BIN = main \
      normal \
      course_m

all: $(BIN)

clean:
	rm -f $(BIN) *.o *.swp

main: main.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt

normal: normal.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt

course_m: course_m.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt

