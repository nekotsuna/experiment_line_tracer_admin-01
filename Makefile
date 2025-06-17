BIN = main \
      normal \
      terminate \
      straight \
      course_m \
      course_n \
      course_q \
      course_t \

all: $(BIN)

clean:
	rm -f $(BIN) *.o *.swp

main: main.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt

normal: normal.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt

terminate: terminate.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt

straight: straight.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt

course_m: course_m.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt

course_n: course_n.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt

course_q: course_q.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt

course_t: course_t.o
	gcc -Wall -pthread -o $@ $^ -lpigpiod_if2 -lrt

