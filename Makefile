BIN = main \
      normal \
      terminate \
      straight \
      course_m \
      course_n \
      course_q \
      course_t \
      course_t_setter \

PIEXISTS = $(shell gcc terminate.c -lpigpiod_if2) 

ifeq ($(PIEXISTS),0)
 PIFLAG = -lpigpiod_if2 -D IN_RASPBERRYPI
else
 PIFLAG =
endif

all: $(BIN)

clean:
	rm -f $(BIN) *.o *.swp

exet:
	./course_t

exets:
	./course_t_setter

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
	gcc -Wall -pthread -o $@ $^ $(PIFLAG) -lrt

course_t_setter: course_t_setter.o
	gcc -Wall -pthread -o $@ $^ $(PIFLAG) -lrt
